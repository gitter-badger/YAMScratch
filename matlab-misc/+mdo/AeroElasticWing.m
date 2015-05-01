classdef AeroElasticWing < handle
	properties
		rho
		Mach
		temp
		V
		sweep
		span
		AR
		taper
		range
		sfc
		nPanel
		nElem
		loadFactor
		diamRoot
		thickRoot
		yieldStress
		sstress
		stwist
		sthick
		aeAxis
		dy
		yPanel
		yElem
		diam
	end
	methods
		%% AeroElasticWing: default constructor
		function [obj] = AeroElasticWing()
			obj.rho = 0.000585114;		% q = .5 * rho * Uinf^2; Density of air (slug/ft^3)
			obj.Mach = 0.84;            % Mach number
			obj.temp = 389.97;          % Temperature (Rankine)
			obj.V = 295.054*obj.Mach*3.28;  % Velocity (ft/sec) = (295.054 m/s)*(3.28 ft/m)
			obj.sweep = 31.5;   % Sweep angle (degrees)
			obj.span = 212.;    % Span (feet)
			obj.AR = 8.7;       % Aspect ratio
			obj.taper = 0.26;   % Taper ratio
			obj.range = 9500;   % Range (nautical miles, nmi)
			obj.sfc = 0.54/3600;    % Specific fuel consumption (1/hr)*(hr/3600s)
			obj.nPanel = 10;
			obj.nElem = obj.nPanel;
			obj.loadFactor = 2.5;
			obj.diamRoot = 180;     % (inches)
			obj.thickRoot = 2.0;    % (inches)
			obj.yieldStress = 45.0E+3;  % (psi)
			obj.sstress = obj.yieldStress;
			obj.stwist = 0.1;
			obj.sthick = .01;
			obj.aeAxis = .25; % Location of aeroelastic axis as percent of local wing chord (from l.e.)
			obj.dy = (obj.span/2)/obj.nPanel;
			obj.yPanel = [obj.dy/2 : obj.dy : obj.span/2-obj.dy/2]';
			obj.yElem = [obj.dy : obj.dy : obj.span/2]';
			obj.diam = obj.diamRoot * (1 - ((1-obj.taper)/(obj.span/2))*obj.yElem); % (inches)
			return
		end
		
		function [L, D, W, alpha, lift, twist, uz, maxStress] = MDA(obj, thick, jigtwist)
				[L, D, W, alpha, lift, twist, uz, maxStress]=...
			    aeroln(obj.rho, obj.Mach, obj.temp, obj.AR, obj.sweep, obj.span, ...
			    obj.taper, obj.nPanel, obj.diam, thick, obj.aeAxis, jigtwist, ...
			    obj.loadFactor, obj.range, obj.sfc, false);
		end

		function [W_fuel] = fuelWeight(obj, Weight, Drag, Lift)
			% compute the fuel weight from the total weight W
			% Solve for Wfuel based on the following:
			% Wfuel = (W - Wfuel) *(exp(range*6076.11549*(sfc*D)/(V*L)) - 1)
			% note: 6076.11549 ft = 1 nmi
			tmp = exp(obj.range*6076.11549*(obj.sfc*Drag)/(obj.V*Lift));
			W_fuel = Weight*(tmp - 1.0)/tmp % (lb)
			return
		end

		function [W_fuel] = OneObjectiveToRuleThemAll(obj, X)
			%provides objective for the 
			assert(isvector(X));
			assert(length(X) == obj.nElem + obj.nPanel)
			local_thick = X(1:obj.nElem);
			local_jigtwist = X(obj.nElem+1:end);
			[L, D, W, alpha, lift, twist, uz, maxStress] = obj.MDA(local_thick, local_jigtwist);
			W_fuel = obj.fuelWeight(W, D, L);

		end

		function [c, ceq] = OneNonlinearConstraintToRuleThemAll(obj, X)
			%equality constraints is only on the root
			assert(isvector(X));
			X_length = length(X);
			assert(X_length == obj.nElem + obj.nPanel)
			local_thick = X(1:obj.nElem);
			local_jigtwist = X(obj.nElem+1:end);
			[L, D, W, alpha, lift, twist, uz, maxStress] = obj.MDA(local_thick, local_jigtwist);
			%fill this with feasible regions
			ceq = zeros(X_length, 1);
			ceq(1) = twist(1);
			c = -1 * ones(X_length, 1);
			c(obj.nElem+1:end) = maxStress - obj.yieldStress;

		end
	end


end