* How to do exercise E15.1 from Stock & Watson.
clear
use "USMacro_Monthly.dta"
* First paste in the data from the Excel file USMacro_Monthly.xls.
gen t = ym(Month,Year)
format t %tm
tsset t

* Generate & inspect variables.  Note that O is already generated and is named oil.
gen ip_growth = 100 * ln(IP/L.IP)
su ip_growth if t >= m(1952m1)
kdensity ip_growth, n(400)  // This looks at the distribution of ip_growth.
su oil, detail
histogram oil, bin(50)
twoway (tsline oil)

* Regress growth of industrial production on 18 lags of the oil price shock variable.
* Here we want to use HAC standard errors, to allow for time-series correlation in the error term.
*   See pages 604 forward from Stock & Watson's book.
* To use HAC standard errors, we use Stata's newey command.
* The newey command requires a "lag(#)" option that specifies the number of lags over which we consider
*   possible correlations in (X-E[X])*epsilon.  Choice of this number is discussed on Stock and Watson
*   pages 607-608, resulting in the guideline in the last two paragraphs of page 607.
di _N-18  // Determine the number of points in time, T, to be used in the regression.  The answer turns out to be 678.
di 0.75 * (_N-18)^(1/3)  // This number, rounded to the nearest integer, is a rule of thumb for the # of lags.
newey ip_growth L(1/18).oil, lag(7)
test L1.oil L2.oil L3.oil L4.oil L5.oil L6.oil L7.oil L8.oil L9.oil L10.oil L11.oil L12.oil L13.oil L14.oil L15.oil L16.oil L17.oil L18.oil

* Making the graphs of the dynamic multipliers and cumulative multipliers, with their
*   confidence intervals, is a bit of a pain.  Here's how you might do it.  I've
*   created a program to help.  You should be able to understand the programs, with
*   the help of the comments here.
* The basic matrix commands in Stata are a bit clunky, and Stata has a more elegant
*   matrix language called "mata", but here I use the clunky versions for simplicity.
* The program creates extra variables named lag, dynamicMult, dm_ciMin, dm_ciMax,
*   cumulativeMult, cm_ciMin, and cm_ciMax.  These have nonmissing values in the first
*   18 rows of data only.
capture program drop getDynamicMultipliers  // If the following program already exists, delete it before recreating it, to avoid an error message that it already exists.
program define getDynamicMultipliers
	* The matrix e(b), after estimation, is a row vector containing the estimates for the lags of oil.
	* The matrix e(V) is the estimated variance-covariance matrix of the parameters.
	* We are going to create a matrix named infoToKeep that will contain:
	*   the lag numbers 1 through 18 in the first column, 
	*   the estimated dynamic multipliers (DMs) in the second column,
	*   the 95% confidence interval min & max values for the DMs in the 3rd and 4th columns,
	*   the estimated cumulative multipliers (CMs) in the 5th column, and
	*   the 95% confidence interval min & max values for the CMs in the 6th and 7th columns.
	matrix b = e(b)'  // Get the estimates as a column vector.  The ' means transpose.  The first 18 elements are the estimates of the first 18 lags of oil.
	matrix V = e(V)  // Get the estimated variance-covariance matrix of the parameter estimates.
	matrix infoToKeep = J(18,7,-999)  // The J(r,c,v) function returns a (r x c) matrix containing the number v everywhere.  We'll replace those values momentarily.
	forvalues i = 1/18 {
		* Lag number
		matrix infoToKeep[`i',1] = `i'  // Put the lag number, i, into row i column 1 of infoToKeep.
		* Estimated dynamic multiplier and confidence interval.
		matrix infoToKeep[`i',2] = b[`i',1]  // Put the estimated dynamic multiplier of the ith lag into row i column 2 of infoToKeep.
		scalar stderr = sqrt(V[`i',`i'])  // Get the standard error for the coefficient of the ith lag.
		// Note: In the following 2 formulas, 1.96 is replaced by its small-sample equivalent: "invttail(e(df_r),.025)", where "e(df_r)" is the degrees freedom in the t-statistic.  The asymptotic normal equivalent would be "invnorm(.975)".
		matrix infoToKeep[`i',3] = b[`i',1] - invttail(e(df_r),.025)*scalar(stderr)  // The minimum value of each confidence interval is its dynamic multiplier minus 1.96 times its standard error.  Put this into row i column 3 of infoToKeep.
		matrix infoToKeep[`i',4] = b[`i',1] + invttail(e(df_r),.025)*scalar(stderr)  // The maximum value of each confidence interval is its dynamic multiplier plus 1.96 times its standard error.  Put this into row i column 4 of infoToKeep.
		* Estimated cumulative dynamic multiplier and confidence interval.
		if `i'==1 {
			local linearCombo L1.oil  // If i is 1, create a local string named linearCombo equal to "L1.oil".
		}
		else {
			local linearCombo `linearCombo'+L`i'.oil  // Keep adding "+L2.oil", etc., after the string linearCombo each time through the loop.
		}
		quietly lincom `linearCombo'  // Get the estimate and standard error for the linear combination L1.oil+L2.oil+..., up to the ith lag.
		  // After the lincom command, r(estimate) contains the estimate and r(se) contains its standard error.
		matrix infoToKeep[`i',5] = r(estimate)  // Put the estimated ith cumulative multiplier into row i column 5 of infoToKeep.
		matrix infoToKeep[`i',6] = r(estimate) - invttail(e(df_r),.025)*r(se)  // The minimum value of each confidence interval is its cumulative multiplier minus 1.96 times its standard error.  Put this into row i column 6 of infoToKeep.
		matrix infoToKeep[`i',7] = r(estimate) + invttail(e(df_r),.025)*r(se)  // The maximum value of each confidence interval is its cumulative multiplier minus 1.96 times its standard error.  Put this into row i column 7 of infoToKeep.
	}
	// At this point, if you "matrix list infoToKeep", you should see that it gives the same coefficient
	//   estimates and confidence intervals as reported in the regression results.  It also has the same
	//   cumulative multiplier estimates and standard errors as reported using lincom commands.
	svmat infoToKeep  // This makes variables from the numbers in the matrix.
	rename infoToKeep1 lag
	label variable lag "Lag (in Months) of Oil Price Shock"
	rename infoToKeep2 dynamicMult
	label variable dynamicMult "Dynamic Multiplier"
	rename infoToKeep3 dm_ciMin
	rename infoToKeep4 dm_ciMax
	rename infoToKeep5 cumulativeMult
	label variable cumulativeMult "Cumulative Multiplier"
	rename infoToKeep6 cm_ciMin
	rename infoToKeep7 cm_ciMax
end

getDynamicMultipliers  // Run the program defined above.

* Make the graph for the dynamic multipliers.
twoway (line dynamicMult lag, lcolor(blue)) (line dm_ciMin lag in 1/18, lcolor(black)) (line dm_ciMax lag in 1/18, lcolor(black)) in 1/18, title(Estimated Dynamic Multipliers and 95% Confidence Intervals) subtitle(additive effects on US % industrial production growth rate) legend(off)

* Make the graph for the cumulative multipliers.
twoway (line cumulativeMult lag, lcolor(blue)) (line cm_ciMin lag in 1/18, lcolor(black)) (line cm_ciMax lag in 1/18, lcolor(black)) in 1/18, title(Estimated Cumulative Multipliers and 95% Confidence Intervals) subtitle(additive effects to date on US % industrial production growth rate) legend(off)
