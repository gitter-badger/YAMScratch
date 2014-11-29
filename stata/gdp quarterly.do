capture log close
clear 
use "UsMacro Quarterly.dta"
gen time = yq(Year,Quarter)
tsset time,quarterly
gen ln_gdp = log(RealGDP)
gen delta_y = D.ln_gdp
gen lag1_y = L.ln_gdp
gen lag2_y = L2.ln_gdp
gen lag3_y = L3.ln_gdp
gen lag4_y = L4.ln_gdp

//generate an AR(1) model
regress ln_gdp L.ln_gdp
eststo AR

//null = 
dfuller ln_gdp, regress trend
//test

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
			quietly regress ln_gdp L.ln_gdp if time < `test_quarter'
			scalar ess_1 = e(rss)
			scalar N_1 = e(N)
			//di scalar(N_1)
			//group time 2 starts at test_quarter
			quietly regress ln_gdp L.ln_gdp if time >= `test_quarter'
			scalar ess_2 = e(rss)
			scalar N_2 = e(N)
			//di scalar(N_2)
			//overall regression
			quietly regress ln_gdp L.ln_gdp
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
ytitle("") xtitle("Time") ///
text(4.5 -40 "1% Level") text(2.8 -40 "5% Level") ///
text(`=scalar(max_chow)+.5' `=scalar(max_f_time)+15' "Q3:1958")




//ADL(1,4) model for delta Yt using lags of delta Rt
local r_lags "D.RealGDP L.D.RealGDP L2.D.RealGDP L3.D.RealGDP"
local adl "D.ln_gdp D.L.ln_gdp `r_lags'"
regress `adl'
eststo ADL

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
// b_F_01 = 3.0940
// b_F_05 = 2.2515
gen dummy_time_ex = max_f_time
format dummy_time_ex %tq
di max_chow
graph twoway (line chow_stat_2 time), yline(3.0940) ylabel(0(20)100) name(graph2, replace) ///
xline(`=scalar(max_f_time)',lp(shortdash)) ///
title("QLR Test for ADL(1,4) Model") ///
subtitle("Test of Break Between Q1:1955 and Q4:2002") ///
legend(label(1 "Chow Statistic")) ///
ytitle("") xtitle("Time") ///
text(7 -40 "1% Level") ///
text(`=scalar(max_chow)+5' `=scalar(max_f_time)+15' "Q2:1966")
