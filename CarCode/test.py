actual = int(raw_input("Enter the heading: "))
desired = int(raw_input("Enterf the desired: "))

angle = actual - desired

if angle > 0:
	if angle > 180:
		print " turn left"
	else:
		print "turn right"
else:
	if angle < -180:
		print "turn right"
	else:
		print "turn left"