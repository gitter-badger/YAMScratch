import math

pressure = [520,427,351,288,236,188,145,109,73,36,0] #pressure in mm of H20
x_meters = [x/2.0 for x in range(0,len(pressure))]
delta = [] # store the result
diff = lambda x,i :x[i]-x[i+1]
length_units = "m"
pressure_units = "mm H20"
try:
	for i in range(0,len(pressure)):
		

		delta.append(diff(pressure,i)/diff(x_meters,i))

except IndexError:
	pass
print "The pressure drops are\n"+"="*60

print "X: %4.2f %s, Pressure = %d %s" %(x_meters[0],length_units,pressure[0],pressure_units)
for index,thing in enumerate(delta):
	print ""
	print " "*4+u'\u0394'+"P/"+u'\u0394'+"x = %4.2f %s/%s" % (thing,pressure_units,length_units)
	print ""
	print "X: %4.2f %s, Pressure = %d %s" %(x_meters[index+1],length_units,pressure[index+1],pressure_units)
print ""
