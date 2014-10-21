

while(True):
	actual = int(raw_input("Enter the actual heading: "))
	desired = int(raw_input("Enter the desired heading: "))
	
	theta = actual - desired

	for i in range(0,360):
		print i - desired
