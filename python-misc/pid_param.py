taui = 0.4
po = 1.0/taui
tau = 0.0929
epsilon = 0.456
wo = 14.2
K = 19.9

ki = (tau*(wo**2)*po)/K

kp = ((tau*(wo**2)) + 2*tau*epsilon*wo*po)/K

kd = (2*tau*epsilon*wo + tau*po -1)/K

print "ki:", ki
print "kp:", kp
print "kd:", kd

