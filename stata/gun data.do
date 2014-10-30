clear

use "Guns.dta"
log using lab6log.txt, text replace
gen ln_vio = log(vio)
tsset stateid year // tell stata we have time series data
//regress against shall
regress ln_vio shall,r
//only state effects
areg ln_vio shall, absorb(stateid) r
//only time effects
areg ln_vio shall, absorb(year) r
//state and time effects
xi:areg ln_vio shall i.year,absorb(stateid) r 

//regress against other control variables
regress ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects
areg ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(stateid) r
//only time effects
areg ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(year) r
//state and time effects
xi:areg ln_vio shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)
//======================================================================//
gen ln_rob = log(rob)
//regress against shall
regress ln_rob shall,r
//only state effects
areg ln_rob shall, absorb(stateid) r
//only time effects
areg ln_rob shall, absorb(year) r
//state and time effects
xi:areg ln_rob shall i.year,absorb(stateid) r 

//regress against other control variables
regress ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects
areg ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(stateid) r
//only time effects
areg ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(year) r
//state and time effects
xi:areg ln_rob shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)
//=================================================================//
gen ln_mur = log(mur)
//regress against shall
regress ln_mur shall,r
//only state effects
areg ln_mur shall, absorb(stateid) r
//only time effects
areg ln_mur shall, absorb(year) r
//state and time effects
xi:areg ln_mur shall i.year,absorb(stateid) r 

//regress against other control variables
regress ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029 ,r
//set up the state fixed effects
areg ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(stateid) r
//only time effects
areg ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029, absorb(year) r
//state and time effects
xi:areg ln_mur shall incarc_rate density avginc pop pb1064 pw1064 pm1029 i.year, absorb(stateid)


log close
