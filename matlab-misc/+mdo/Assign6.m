clear('all');
clc
import mdo.*

wing = AeroElasticWing()

tiptwist = -1;

thick = wing.thickRoot * (1 - ((1-wing.taper)/(wing.span/2))*wing.yElem);   % (inches)
jigtwist = wing.yPanel *(tiptwist/(wing.span/2)); % (degrees)

[L, D, W, alpha, lift, twist, uz, maxStress] = wing.MDA(thick, jigtwist);

L
D
W
alpha
maxStress

tmp = exp(wing.range*6076.11549*(wing.sfc*D)/(wing.V*L));
Wfuel = W*(tmp - 1.0)/tmp % (lb)

otherW = wing.fuelWeight(W, D, L)
