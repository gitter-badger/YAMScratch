classdef AirPlane
	properties
		beam;			%
		chord;			%
		W_0 = 4940;			%Initial weight in newtons
		W_w = NaN; 				%Weight of wing in newtons
		N_ult = 2.5;		%
		t_over_c 0.12;		%
		rho = 1.23;			%density of air
		mu = 17.8e-6;		%viscosity of air
		Velocity = 35;		%airspeed
		Surf_area = 11.8;		%wing surface area
		Surf_wet;		%wing wetted area
		k = 1.2;				%form factor
		Co_lift;		%Coefficient of Lift
		e; 				%Oswald efficiency factor	
	end
	methods
		function obj = AirPlane(obj)
			obj.Surf_wet = 2.05*obj.Surf_area;

		end
		function validateWeight(obj)

		end
	end

end