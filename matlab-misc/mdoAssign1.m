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
x_prev = 2.8;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
%alpha_init = input('Enter inital step length: ');
alpha_max = 3;
hold on
plot(x_prev,simple(x_prev),'gd');
hold on
%do a bad thing and have infinite while loop to simulate do while
[f_init,g_init] = objective(x_prev);
%store each run of function for plotting convergence
g_metric = [];
x_metric = [x_prev];
while true
	%compute the descent direction of the objective function
	%in this case we directly evaluate the gradient and then normalize
	[f_this,g_this] = objective(x_prev);
	p = -g_this./norm(g_this);
	step = mdoLineSearch(objective, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%jump to this point
	x_curr = x_prev + p*step;
	x_metric(end+1) = x_curr;
	%these two never change
	hold on
	plot(x_curr,simple(x_curr),'rd');
	hold on
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = objective(x_curr);
	if (norm(g_curr) < norm(g_init)*1e-25)
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
		break;
	else
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
	end
	x_prev = x_curr;
end
%calculate the difference from actual point
x_metric = abs(x_metric - 0.5);
%test by golden section method

fig1 = figure;
figure(fig1);
a = -2;
b = 4;
epsilon = 1e-7;
[left,right,cost_data] = mdoGoldenSection(objective,a,b,epsilon);
tolerance = (b-a)*1e-3; %visable convergence intervals
%figure out the relative error metric
gold_metric = abs(cost_data(:,1) - cost_data(:,3));
mdoPlotGoldenSectionData(cost_data, fig1, tolerance, 0.5) % using specialized plotting function
%plotting the function values
x_vals = [a-.5:0.01:b+.5];
y_vals = simple(x_vals);
plot(x_vals,y_vals)

%compare the results from the line search
fig_metric = figure;
figure(fig_metric);
semilogy(1:length(g_metric), g_metric,'kd:')
hold on

semilogy(1:length(x_metric), x_metric, 'ko-.')
hold on
semilogy(1:length(gold_metric), gold_metric, 'rd--')
hold on

title('Line search and golden section search')
xlabel('Iterations k')
legend('|g_k|/|g_0|','|x_k - x.|', '\delta_k');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
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

cons_1 = k*(S_wet/S)*0.074;
cons_1 = cons_1 * (rho*Velocity*sqrt(S)/mu)^-0.2;
cons_2 = C_l^2/(pi*e);
Cd = @(A) (cons_1*(A^0.1)+ cons_2/A);
Cd_prime = @(A) (cons_1*A^-0.9/10 - cons_2/A^2);
%Reynolds = @(l_char) (rho*Velocity*l_char/mu);
%c = @(A)(sqrt(S/A));
%C_f = @(A) (0.074*(Reynolds(c(A))^-0.2));
%C_d = @(A) (k*C_f(A)*S_wet/S + C_l^2/(pi*A*e));

Aspect_ratios = [1:0.001:50];
drag_co = zeros(length(Aspect_ratios),1);
for index = 1:length(Aspect_ratios)
	drag_co(index) = Cd(Aspect_ratios(index));
end
plot(Aspect_ratios,drag_co,'b')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
this_obj = @(A) (deal(Cd(A),Cd_prime(A)));
x_prev = 4;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
%alpha_init = input('Enter inital step length: ');
alpha_max = 3;
hold on
plot(x_prev,Cd(x_prev),'gd');
hold on
%do a bad thing and have infinite while loop to simulate do while
[f_init,g_init] = this_obj(x_prev);
%store each run of function for plotting convergence
sec_g_metric = [];
x_metric = [x_prev];
counter = 1;
while counter < 100
	%compute the descent direction of the objective function
	%in this case we directly evaluate the gradient and then normalize
	[f_this,g_this] = this_obj(x_prev);
	p = -g_this./norm(g_this);
	step = mdoLineSearch(this_obj, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%jump to this point
	x_curr = x_prev + p*step;
	x_metric(end+1) = x_curr;
	%these two never change
	hold on
	plot(x_curr,Cd(x_curr),'rd');
	hold on
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = this_obj(x_curr);
	if (norm(g_curr) < norm(g_init)*1e-25)
		sec_g_metric(end+1) = (norm(g_curr)/norm(g_init));
		break;
	else
		sec_g_metric(end+1) = (norm(g_curr)/norm(g_init));
	end
	x_prev = x_curr;
	counter = counter + 1;
end
%calculate the difference from actual point
x_metric = abs(x_metric - 28.39424799779898);
%test by golden section method


a = 3;
b = 50;
epsilon = 1e-7;
[left,right,sec_cost_data] = mdoGoldenSection(this_obj,a,b,epsilon);
tolerance = (b-a)*1e-7; %visable convergence intervals
%figure out the relative error metric
gold_metric = abs(sec_cost_data(:,1) - sec_cost_data(:,3));
mdoPlotGoldenSectionData(sec_cost_data, fig2, tolerance, 0.001) % using specialized plotting function
%plotting the function values
x_vals = [a-.5:0.01:b+.5];
y_vals = zeros(length(x_vals),1);
for index = 1:length(x_vals)
	y_vals(index) = Cd(x_vals(index));
end
plot(x_vals,y_vals)

%compare the results from the line search
r_fig_metric = figure;
figure(r_fig_metric);
semilogy(1:length(sec_g_metric), sec_g_metric,'gd:')
hold on

semilogy(1:length(x_metric), x_metric, 'ko-.')
hold on
semilogy(1:length(gold_metric), gold_metric, 'rd--')
hold on

title('Second Line search and golden section search')
xlabel('Iterations k')
legend('|g_k|/|g_0|','|x_k - x.|', '\delta_k');