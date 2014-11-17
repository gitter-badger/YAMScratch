def conf_intv(mean,se,level):
	print "[ {} , {} ]".format(mean-se*level,mean+se*level)

while True:
	mean = float(raw_input("mean: "))
	se = float(raw_input("standard error: "))
	level = float(raw_input("level: "))
	conf_intv(mean,se,level)