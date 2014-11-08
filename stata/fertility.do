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

esttab using results, rtf replace

regress morekids samesex, vce(robust)
 
