clear
use "Guns.dta"
log using lab5log.txt, text replace
gen ln_vio = log(vio)
//regress against shall
regress ln_vio shall,r
//regress against other control variables
regress ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects

tsset stateid year // tell stata we have time series data

xi:areg ln_vio shall i.year,absorb(stateid) r 

xi:areg ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)
/*
areg ln_vio shall,absorb(stateid) r
forvalues t=77/99 {
	gen foo_`t' = 1 if year == `t'
	replace foo_`t' = 0 if year != `t'
}
areg ln_vio shall foo_77 foo_78 foo_79 foo_80 foo_81 foo_82 foo_83 foo_84 foo 

*/
gen ln_rob = log(rob)
//regress against shall
regress ln_rob shall,r
//regress against other control variables
regress ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects



xi:areg ln_rob shall i.year,absorb(stateid) r 

xi:areg ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)
//=================================================================//
gen ln_mur = log(mur)
//regress against shall
regress ln_mur shall,r
//regress against other control variables
regress ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects

xi:areg ln_mur shall i.year,absorb(stateid) r 

xi:areg ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)

log close
