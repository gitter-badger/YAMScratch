capture log close
clear
use "JEC.dta"
log using lab8log.txt, text replace

//make log variables for elasticity
gen ln_price = log(price)
gen ln_quantity = log(quantity)

regress ln_quantity ln_price ice seas2 seas3 seas4 seas5 seas6 seas7 seas8 seas9 seas10 seas11 seas12, vce(robust)
test seas2 seas3 seas4 seas5 seas6 seas7 seas8 seas9 seas10 seas11 seas12
regress ln_quantity ln_price ice

ivregress 2sls ln_quantity ice seas2 seas3 seas4 seas5 seas6 seas7 seas8 seas9 seas10 seas11 seas12 (ln_price = cartel ), vce(robust)
estat firststage
estat endogenous

regress ln_price cartel ice seas2 seas3 seas4 seas5 seas6 seas7 seas8 seas9 seas10 seas11 seas12

log close
