function [W_fuel] = FuelWeightObj(Weight, Drag, Velocity, Lift, Range, sfc)
	% compute the fuel weight from the total weight W
	% Solve for Wfuel based on the following:
	% Wfuel = (W - Wfuel) *(exp(range*6076.11549*(sfc*D)/(V*L)) - 1)
	% note: 6076.11549 ft = 1 nmi
	tmp = exp(Range*6076.11549*(sfc*Drag)/(Velocity*Lift));
	W_fuel = Weight*(tmp - 1.0)/tmp % (lb)
	return
end
