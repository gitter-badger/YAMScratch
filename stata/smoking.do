capture log close
clear
use "Smoking.dta"
log using lab7log.txt, text replace
gen age2 = age^2
estpost summarize smoker smkban female age age2 hsdrop hsgrad colsome colgrad black hispanic ,listwise
esttab, cells("mean sd min max count") nomtitle nonumber
//find the expected value for smoking
mean smoker
ttest smoker, by(smkban)
quietly eststo LPM_simple: regress smoker smkban , vce(robust)

quietly eststo PRO_full: probit smoker smkban female age age2 hsdrop hsgrad colsome colgrad black hispanic, vce(robust)
//prob for Mr. A :white, non_hispanic, 20 years old, high school dropout
scalar z_noban = _b[_cons]+_b[age]*20+_b[age2]*(20^2)+_b[hsdrop]
di normprob(scalar(z_noban))
scalar z_ban = _b[_cons]+_b[age]*20+_b[age2]*(20^2)+_b[hsdrop]+_b[smkban]
di normprob(scalar(z_ban))
//prob for Ms. B : female, black, 40 years old, college grad
scalar z_noban = _b[_cons]+_b[female]+_b[age]*40+_b[age2]*(40^2)+_b[black]+_b[colgrad]
di normprob(scalar(z_noban))
scalar z_ban = _b[_cons]+_b[female]+_b[age]*40+_b[age2]*(40^2)+_b[black]+_b[colgrad]+_b[smkban]
di normprob(scalar(z_ban))

test hsdrop hsgrad colsome colgrad

quietly eststo LPM_full: regress smoker smkban female age age2 hsdrop hsgrad colsome colgrad black hispanic, vce(robust)
//repeat above for LPM model
	//prob for Mr. A :white, non_hispanic, 20 years old, high school dropout
	scalar z_noban = _b[_cons]+_b[age]*20+_b[age2]*(20^2)+_b[hsdrop]
	di scalar(z_noban)
	scalar z_ban = _b[_cons]+_b[age]*20+_b[age2]*(20^2)+_b[hsdrop]+_b[smkban]
	di scalar(z_ban)
	//prob for Ms. B : female, black, 40 years old, college grad
	scalar z_noban = _b[_cons]+_b[female]+_b[age]*40+_b[age2]*(40^2)+_b[black]+_b[colgrad]
	di scalar(z_noban)
	scalar z_ban = _b[_cons]+_b[female]+_b[age]*40+_b[age2]*(40^2)+_b[black]+_b[colgrad]+_b[smkban]
	di scalar(z_ban)

test hsdrop hsgrad colsome colgrad	
	
capture esttab LPM_simple LPM_full PRO_full using output1,rtf b(a3) se(a3) star(+ 0.10 * 0.05 ** 0.01 *** 0.001) r2(3) ar2(3) scalars(F chi2 r2_p cmd) nogaps
if _rc == 602{
	esttab LPM_simple LPM_full PRO_full using output1 ,rtf replace b(a3) se(a3) star(+ 0.10 * 0.05 ** 0.01 *** 0.001) r2(3) ar2(3) scalars(F chi2 r2_p cmd) nogaps
}


seq pred_age,f(18) t(65)
gen pred_prob = _b[_cons]+_b[age]*pred_age+_b[colgrad]

scatter smoker age || line pred_prob pred_age
log close
