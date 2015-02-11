%Script to run the line search on the simple test platform
%Copyright (c) 2015 Isaiah Bell
%All rights reserved
clear
clc
close all;
%setup test function
simple = @(x) (x.^2-x);
%prepare plotting the function
test_x = [-2:0.01:3];
simple_y = simple(test_x);
plot(test_x,simple_y)
%create an objective function handle to pass around
%this returns [f, f'] of the simple function above
objective = @(x)(deal((x^2-x),(2*x-1))) 
%configure the linesearch parameters here
x_prev = 2;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
%alpha_init = input('Enter inital step length: ');
alpha_max = 3;

%do a bad thing and have infinite while loop to simulate do while
while true
	%compute the descent direction of the objective function
	%in this case we directly evaluate the gradient and then normalize
	[f_init,g_init] = objective(x_prev);
	p = -g_init./norm(g_init);

	step = mdoLineSearch(objective, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%jump to this point
	x_curr = x_prev + p*step;
	%these two never change
	hold on
	plot(x_curr,simple(x_curr),'rd');
	hold on
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = objective(x_curr);
	if (norm(g_curr) < norm(g_init)*1e-6)
		break;
	else
		disp(norm(g_curr)/norm(g_init))
	end
	x_prev = x_curr
end

%test by golden section method

fig1 = figure;
figure(fig1);
a = -2;
b = 4;
epsilon = 1e-7;
[left,right,cost_data] = mdoGoldenSection(objective,a,b,epsilon);
%create a z_buffer to graphically show golden section convergence
tolerance = (b-a)*1e-2; %visable convergence intervals
[r,c] = size(cost_data);
for index = 1:r
	if (cost_data(index,3) - cost_data(index,1) < tolerance)
		stop_index = index;
		break
	end
end

top_z = max([cost_data(1,1),cost_data(1,3)]);
disp(top_z)

x_vals = [a-.5:0.01:b+.5];
y_vals = simple(x_vals);
plot(x_vals,y_vals)

%test the real question
fig2 = figure;
figure(fig2);

%create the drag equation
rho = 1.23;	%density of air kg/m^3
mu = 17.8 * 1e-6; % viscosity of air kg/(m sec)
Velocity = 35; %airspeed m/sec
S = 11.8; %planform area m^2
S_wet = 2.05*S; % wing wetted area m^2
k = 1.2; %form factor
C_l = 0.3; %lift coefficient
e = 0.96; %Oswald efficiency factor

Reynolds = @(l_char) (rho*Velocity*l_char/mu);

