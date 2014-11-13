capture log close
clear
use "fertility.dta"
log using lab8fer.txt, text replace

regress weeksm1 morekids, vce(robust)
eststo

ivregress 2sls weeksm1 (morekids = samesex), vce(robust)
eststo
estat endogenous
estat firststage

ivregress 2sls weeksm1 agem1 black hispan othrace (morekids = samesex), vce(robust)
eststo
estat endogenous
estat firststage

esttab using results, rtf replace b(a3) se(a3) star(+ 0.10 * 0.05 ** 0.01 *** 0.001) r2(3) ar2(3) scalars(F chi2 r2_p cmd)

regress morekids samesex, vce(robust)
 
