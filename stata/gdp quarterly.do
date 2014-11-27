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

//generate a ar(1) model
regress ln_gdp L.ln_gdp
eststo AR

//null = 
dfuller ln_gdp, regress trend
//test

//test the QLR
//create a dummy variable for the break period
gen chow_stat = .
scalar k = 2 // the number of paramters we estimate for AR(1)
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
			quietly replace chow_stat = ((ess_c - (ess_1+ess_2))/k)/((ess_1 + ess_2)/(N_1 + N_2 - 2*k)) if time == `test_quarter'
		}
	}
	scalar last_date = `test_quarter'
}
line(chow_stat time),name(first)

//ADL(1,4) model for delta Yt using lags of delta Rt
local r_lags "D.RealGDP L.D.RealGDP L2.D.RealGDP L3.D.RealGDP"
local adl "D.ln_gdp D.L.ln_gdp `r_lags'"
regress `adl'
eststo ADL

//test the QLR
//create a dummy variable for the break period
gen chow_stat_2 = .
scalar k = 5 // the number of paramters we estimate for ADL(1,4)

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
			quietly replace chow_stat_2 = ((ess_c - (ess_1+ess_2))/k)/((ess_1 + ess_2)/(N_1 + N_2 - 2*k)) if time == `test_quarter'
		}
	}
	scalar last_date = `test_quarter'
}
line(chow_stat_2 time), name(second)

