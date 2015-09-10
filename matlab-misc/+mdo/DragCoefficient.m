function [f,f_prime] = DragCoefficient(A)
	%%create the drag equation
	rho = 1.23;	%density of air kg/m^3
	mu = 17.8 * 1e-6; % viscosity of air kg/(m sec)
	Velocity = 35; %airspeed m/sec
	S = 11.8; %planform area m^2
	S_wet = 2.05*S; % wing wetted area m^2
	k = 1.2; %form factor
	C_l = 0.3; %lift coefficient
	e = 0.96; %Oswald efficiency factor
	cons_1 = k*(S_wet/S)*0.074;
	cons_1 = cons_1 * (rho*Velocity*sqrt(S)/mu)^-0.2;
	cons_2 = C_l^2/(pi*e);
	f = (cons_1*(A^0.1)+ cons_2/A);
	f_prime = (cons_1*A^-0.9/10 - cons_2/A^2);
	%%Below is equivalent expression using lambdas 
	%Reynolds = @(l_char) (rho*Velocity*l_char/mu);
	%c = @(A)(sqrt(S/A));
	%C_f = @(A) (0.074*(Reynolds(c(A))^-0.2));
	%C_d = @(A) (k*C_f(A)*S_wet/S + C_l^2/(pi*A*e));
	return
end