classdef AirPlane
	properties(Constant)
		W_0 = 4940;			%Initial weight in newtons
		W_w = NaN; 			%Weight of wing in newtons
		N_ult = 2.5;		%
		t_over_c 0.12;		%
		rho = 1.23;			%density of air
		mu = 17.8e-6;		%viscosity of air
		Velocity = 35;		%airspeed
		k = 1.2;			%form factor
		e = 0.96; 			%Oswald efficiency factor	
	end
	methods
		%default constructor
		function obj = AirPlane(obj)

		end
	end

	methods(Static)
		function S_wet = getWettedSurface(S)
			S_wet = 2.05 * S;
			return
		end
		function beam = getBeam(S, A)
			beam = sqrt(A*S);
			return
		end
		function chord = getChord(S, A)
			chord = sqrt(S/A);
			return
		end
		function getDragTotal(obj, S, A)
			
		end
	end

end