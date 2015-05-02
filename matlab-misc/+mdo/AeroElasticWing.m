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
		nDOF
		E
		matRho
		R
		gamma
		a
		Uinf
		q
		S
		Offsets
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
			obj.nDOF = 6 * obj.nElem;
			obj.E = 12E6;        % English
			obj.matRho = 0.105;  % English
			obj.R = 1716.562845; % English
			obj.gamma = 1.4;
			obj.a = sqrt(obj.gamma*obj.R*obj.temp);
			obj.Uinf = obj.Mach * obj.a;
			obj.q = 0.5 * obj.rho * obj.Uinf^2;
			obj.S = obj.span^2 / obj.AR;
			%creat a table of primary keys for easy indexing
			obj.Offsets(1) = 1; % start of thick
			obj.Offsets(2) = obj.Offsets(1) + obj.nElem; % start of jigtwist
			obj.Offsets(3) = obj.Offsets(2) + obj.nPanel; % start of displacements
			obj.Offsets(4) = obj.Offsets(3) + obj.nDOF; % start of gamma
			obj.Offsets(5) = obj.Offsets(4) + obj.nPanel; %start of angle
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
			W_fuel = Weight*(tmp - 1.0)/tmp; % (lb)
			return
		end

		function [W_fuel] = OneObjectiveToRuleThemAll(obj, X)
			%provides objective for the 
			assert(isvector(X));
			assert(length(X) == obj.nElem + obj.nPanel)
			%X
			local_thick = X(1:obj.Offsets(2)-1);
			local_jigtwist = X(obj.Offsets(2):obj.Offsets(3)-1);
			[L, D, W, alpha, lift, twist, uz, maxStress] = obj.MDA(local_thick, local_jigtwist);
			W_fuel = obj.fuelWeight(W, D, L);

		end

		function [c, ceq] = OneNonlinearConstraintToRuleThemAll(obj, X)
			%equality constraints is only on the root
			assert(isvector(X));
			X_length = length(X);
			assert(X_length == obj.nElem + obj.nPanel)
			%X
			local_thick = X(1:obj.Offsets(2)-1);
			local_jigtwist = X(obj.Offsets(2):obj.Offsets(3)-1);
			[L, D, W, alpha, lift, twist, uz, maxStress] = obj.MDA(local_thick, local_jigtwist);
			%fill this with feasible regions
			ceq = zeros(X_length, 1);
			ceq(1) = twist(1);
			c = -1 * ones(X_length, 1);
			%hack to get around equality constraint
			%c(1:obj.nElem) = -local_thick - 1e-10 
			c(1:obj.nElem) = (maxStress - obj.yieldStress);
			return
		end

		function [W_fuel] = IDFObjective(obj, X)
			assert(isvector(X));
			X_length = length(X);
			local_thick = X(1:obj.Offsets(2)-1);
			local_jigtwist = X(obj.Offsets(2):obj.Offsets(3)-1);
			local_u = X(obj.Offsets(3): obj.Offsets(4)-1);
			local_gamma = X(obj.Offsets(4): obj.Offsets(5)-1);
			local_alpha = X(obj.Offsets(5));
			%dont bother checking the length for now
			
			return
		end

		function [c, ceq] = MalebolgeConstraint(obj,X)
			assert(isvector(X))
			local_thick = X(1:obj.Offsets(2)-1);
			local_jigtwist = X(obj.Offsets(2):obj.Offsets(3)-1);
			local_u = X(obj.Offsets(3): obj.Offsets(4)-1);
			local_gamma = X(obj.Offsets(4): obj.Offsets(5)-1);
			local_alpha = X(obj.Offsets(5));
			
			Gamma = obj.AeroDiscipline(local_twist, local_alpha);
			U = obj.StructDiscipline(local_thick, local_gamma);
			Alpha = obj.AttackDiscipline(local_thick, local_gamma);

		end

		function [y_prime] = AeroDiscipline(obj, twist, alpha)
			AIC = aeroAIC(obj.AR, obj.nPanel, obj.Mach, obj.taper, obj.sweep);
			v = zeros(nPanel, 1);
  			v(1) = (alpha+twist(1)) * pi/180; 
  			for i = 2 : obj.nPanel
    			v(i) = (alpha+twist(i)) * pi/180;  % radians
 			end 
 			Gamma = AIC \ v;

 			y_prime = Gamma;
 			return
		end

		function [y_prime] = StructDiscipline(obj, thick, Gamma)

			K = stiffness(obj.sweep, obj.span*12, obj.diam, thick, obj.E, obj.nElem);
			lift = seclift(Gamma, obj.nPanel, obj.q, obj.span);
			f = force(lift, obj.aeAxis, obj.nDOF, obj.nPanel, obj.AR, obj.sweep, obj.taper, obj.span);

			u = K \ f;
			y_prime = u;
			return
		end

		function [y_prime] = AttackDiscipline(obj, thick, Gamma)

			lift = seclift(Gamma, obj.nPanel, obj.q, obj.span);
  			f = force(lift, obj.aeAxis, obj.nDOF, obj.nPanel, obj.AR, obj.sweep, obj.taper, obj.span);
			L = totalLift(Gamma, obj.nPanel, obj.q, obj.S, obj.AR, obj.sweep, obj.Mach);

			W = weightStruct(sweep, span*12, obj.diam, thick, matRho, nElem);

		end
	end
end