capture log close
clear
use "Names.dta"
log using lab9log.txt, text replace

sort black
by black: summarize call_back

by black: regress call_back high,vce(robust)

regress call_back high

ttest high,by (black)
eststo
ttest female,by (black)
eststo
ttest college,by (black)
eststo
ttest chicago,by (black)
eststo
ttest specialskills, by (black)
eststo
ttest computerskills, by (black)
eststo

capture esttab using output_ttest, rtf replace
