classdef AirPlane
	properties(Constant)
		W_0 = 4940;			%Initial weight in newtons
		N_ult = 2.5;		%
		t_over_c  = 0.12;	%
		rho = 1.23;			%density of air
		mu = 17.8e-6;		%viscosity of air
		k = 1.2;			%form factor
		e = 0.96; 			%Oswald efficiency factor
	end
	properties
		S_area_wet;
		S_area;
		beam;
		chord;
		Velocity; 			%airspeed
	end
	methods
		function obj = AirPlane(A,S)
			assert(isscalar(S));
			assert(isscalar(A));
			obj.S_area = S;
			obj.S_area_wet = 2.05 * S;
			obj.beam = sqrt(A*S);
			obj.chord = sqrt(S/A);
			obj.Velocity = 35;
		end
	end
end