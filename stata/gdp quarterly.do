capture log close
clear 
use "UsMacro Quarterly.dta"
gen time = yq(Year,Quarter)
tsset time,quarterly
gen ln_gdp = log(RealGDP)
gen delta_y = D.ln_gdp
gen delta_r = D.RealGDP
summarize
//twoway (line delta_y time) ,name(graph3,replace)

gen lag1_y = L.ln_gdp
gen lag2_y = L2.ln_gdp
gen lag3_y = L3.ln_gdp
gen lag4_y = L4.ln_gdp

//generate an AR(1) model
local ar1 = "delta_y D.lag1_y" 
regress `ar1', vce(robust)
eststo AR
//null = unit root
dfuller delta_y, regress trend

//test the QLR
//create a dummy variable for the break period
gen chow_stat = .
scalar k = 2 // the number of paramters we estimate for AR(1)
scalar max_f_time = 0
scalar max_chow = 0
//local top_chow ((`ess_c' - (`ess_1'+`ess_2'))/k)
//local bot_chow ((`ess_1' + `ess_2')/(`N_1' + `N_2' - 2*k))
forvalues test_quarter = `=q(1947q1)'/`=q(2009q4)' {
	if `test_quarter' >= `=q(1955q1)'{
		if `test_quarter' <= `=q(2002q4)' {
			//group time 1
			quietly regress `ar1' if time < `test_quarter'
			scalar ess_1 = e(rss)
			scalar N_1 = e(N)
			//di scalar(N_1)
			//group time 2 starts at test_quarter
			quietly regress `ar1' if time >= `test_quarter'
			scalar ess_2 = e(rss)
			scalar N_2 = e(N)
			//di scalar(N_2)
			//overall regression
			quietly regress `ar1'
			scalar ess_c = e(rss)
			//calculate the chow statistic
			scalar temp_chow = ((ess_c - (ess_1+ess_2))/k)/((ess_1 + ess_2)/(N_1 + N_2 - 2*k))
			quietly replace chow_stat = temp_chow if time == `test_quarter'
			if temp_chow > max_chow {
				scalar max_f_time = `test_quarter'
				scalar max_chow = temp_chow
			}
		}
	}
	scalar last_date = `test_quarter'
}
gen dummy_time_ex1 = max_f_time
format dummy_time_ex %tq
//1958Q2 is time = -6
// a_F_01 = 4.6921+1
// a_F_05 = 3.0324

graph twoway (line chow_stat time), yline(4.6921 3.0324, lp(solid)) /// 
xline(`=scalar(max_f_time)',lp(shortdash)) ylabel(0(1)5) ///
title("QLR Test for AR(1) Model") ///
subtitle("Test of Break Between Q1:1955 and Q4:2002") ///
legend(label(1 "Chow Statistic")) ///
ytitle("Chow Statistic") xtitle("Time") ///
text(4.5 -40 "1% Level") text(2.8 -40 "5% Level") ///
text(`=scalar(max_chow)+.5' `=scalar(max_f_time)+15' "Q1:2001")

//ADL(1,4) model for delta Yt using lags of delta Rt
local r_lags "D.TBillRate L.D.TBillRate L2.D.TBillRate L3.D.TBillRate"
local adl "D.ln_gdp D.L.ln_gdp `r_lags'"
regress `adl' ,vce(robust)
eststo ADL_total

//test the QLR
//create a dummy variable for the break period
gen chow_stat_2 = .
scalar k = 5 // the number of paramters we estimate for ADL(1,4)
scalar max_chow = 0 //reuse the max routine
forvalues test_quarter = `=q(1947q1)'/`=q(2009q4)' {
	if `test_quarter' >= `=q(1955q1)'{
		if `test_quarter' <= `=q(2002q4)' {
			//group time 1
			quietly regress `adl' if time < `test_quarter'
			scalar ess_1 = e(rss)
			scalar N_1 = e(N)
			//di scalar(N_1)
			//group time 2 starts at test_quarter
			quietly regress `adl' if time >= `test_quarter'
			scalar ess_2 = e(rss)
			scalar N_2 = e(N)
			//di scalar(N_2)
			//overall regression
			quietly regress `adl'
			scalar ess_c = e(rss)
			//calculate the chow statistic
			scalar temp_chow = ((ess_c - (ess_1+ess_2))/k)/((ess_1 + ess_2)/(N_1 + N_2 - 2*k))
			quietly replace chow_stat_2 = temp_chow if time == `test_quarter'
			if temp_chow > max_chow {
				scalar max_f_time = `test_quarter'
				scalar max_chow = temp_chow
			}
		}
	}
	scalar last_date = `test_quarter'
}
//use the right side of break version of the model for forecasting
regress `adl' if time >= max_f_time, vce(robust)
eststo ADL

// b_F_01 = 3.0940
// b_F_05 = 2.2515
gen dummy_time_ex = max_f_time
format dummy_time_ex %tq
di max_chow
graph twoway (line chow_stat_2 time), yline(3.0940) ylabel(0(1)5.4) name(graph2, replace) ///
xline(`=scalar(max_f_time)',lp(shortdash)) ///
title("QLR Test for ADL(1,4) Model") ///
subtitle("Test of Break Between Q1:1955 and Q4:2002") ///
legend(label(1 "Chow Statistic")) ///
ytitle("Chow Statistic") xtitle("Time") ///
text(3.3 -40 "1% Level") ///
text(`=scalar(max_chow)+0.3' `=scalar(max_f_time)+15' "Q2:1966")

//outputs
capture esttab using outputs,rtf replace b(a3) se(a3) ///
star(+ 0.10 * 0.05 ** 0.01 *** 0.001) r2(3) ar2(3) scalars(F)

//forecasts
gen buff1 = .
forecast create AR_cast, replace
forecast estimates AR, names(_delta_GDP)
forecast solve, prefix(AR_) begin(tq(1990q1)) end(tq(2009q4))

gen buff2 = .

forecast create ADL_cast, replace
forecast estimates ADL, names(_delta2_GDP)
forecast solve, prefix(ADL_) begin(tq(1990q1)) end(tq(2009q4))

gen buff3 = .
gen naive_est = (LD.ln_gdp + L2D.ln_gdp + L3D.ln_gdp + L4D.ln_gdp)/4 if time >= tq(1990q1)

gen sq_err_AR = (AR__delta_GDP - delta_y)^2 if time >= tq(1990q1)
gen sq_err_ADL = (ADL__delta2_GDP - delta_y)^2 if time >= tq(1990q1)
gen sq_err_NAIVE = (naive_est - delta_y)^2 if time >= tq(1990q1)

scalar time_forecasted = tq(2009q4)-tq(1990q1)+1

quietly summarize sq_err_AR
local ar_RMSFE = sqrt(r(sum)/time_forecasted)
di `ar_RMSFE'

quietly summarize sq_err_ADL
local adl_RMSFE = sqrt(r(sum)/time_forecasted)
di `adl_RMSFE'

quietly summarize sq_err_NAIVE
local naive_RMSFE = sqrt(r(sum)/time_forecasted)
di `naive_RMSFE'

