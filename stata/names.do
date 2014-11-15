capture log close
clear
use "Names.dta"
log using lab9log.txt, text replace

sort black
by black: summarize call_back

by black: regress call_back high,vce(robust)

regress call_back high

ttest high,by (black)
ttest female,by (black)
ttest college,by (black)
ttest chicago,by (black)
