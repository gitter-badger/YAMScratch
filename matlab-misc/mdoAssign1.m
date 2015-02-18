%Script to run the line search on the simple test platform
%Copyright (c) 2015 Isaiah Bell
%All rights reserved
clear
clc
close all;
fig1 = figure;
figure(fig1);
%setup test function
simple = @(x) (x.^2-x);
%prepare plotting the function
test_x = -2:0.01:3;
simple_y = simple(test_x);
plot(test_x,simple_y)
hold on
%create an objective function handle to pass around
%this returns [f, f'] of the simple function above
objective = @(x)(deal((x^2-x),(2*x-1)));
%configure the linesearch parameters here
x_prev = -1;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = .9;
%alpha_init = input('Enter inital step length: ');
alpha_max = 3;
plot(x_prev,simple(x_prev),'ks');
hold on
%%%%%%%%%%%%%%%%%% LINESEARCH BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[f_init,g_init] = objective(x_prev);
%store each run of function for plotting convergence
g_metric = [];
x_metric = x_prev;
%do a bad thing and have infinite while loop to simulate do while
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
	if (norm(g_curr) < norm(g_init)*1e-6)
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
		break;
	else
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
	end
	x_prev = x_curr;
end
%calculate the difference from actual point
x_metric = abs(x_metric - 0.5);

%%%%%%%%%%%%%%%%%% GOLDEN SECTION BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fig_gold1 = figure;
figure(fig_gold1);
a = -1;
b = 2;
epsilon = 1e-7;
[left,right,cost_data] = mdoGoldenSection(objective,a,b,epsilon);
tolerance = (b-a)*1e-3; %visable convergence intervals
x_vals = a-1:0.01:b+1.5;
y_vals = simple(x_vals);
plot(x_vals,y_vals)
hold on
plot(0.5,simple(0.5),'k*')

legend('f(x)')
xlabel('x')
bat2 = ylabel('f(x)','Rotation',0);
set(bat2,'Units','Normalized','Position',[-0.07 0.5 0]);
%figure out the relative error metric
gold_metric = abs(cost_data(:,1) - cost_data(:,3));
mdoPlotGoldenSectionData(cost_data, fig_gold1, tolerance, 0.5) % using specialized plotting function
%plotting the function values


%compare the results from the line search
figure(fig1);
legend_1 = legend('f(x)','x_0','x_k');
xlabel('x')
bat1 = ylabel('f(x)','Rotation',0);
set(bat1,'Units','Normalized','Position',[-0.07 0.5 0]);


fig_metric = figure;
figure(fig_metric);
semilogy(2:length(g_metric)+1, g_metric,'kd:')
hold on
semilogy(1:length(x_metric), x_metric, 'ko-.')
hold on
semilogy(1:length(gold_metric), gold_metric, 'rd--')
hold on
%title('Line search and golden section search')
xlabel('Iterations k')
legend('|g_k|/|g_0|','|x_k - x.|', '\delta_k');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%							QUESTION 2 									%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
num_evals = mdoCounter;

this_obj = mdoEvalCounter(@mdoDragCoefficient,num_evals);

Aspect_ratios = 1:0.001:50;
drag_co = zeros(length(Aspect_ratios),1);
for index = 1:length(Aspect_ratios)
	drag_co(index) = mdoDragCoefficient(Aspect_ratios(index));
end
%get one figure for golden section
fig_gold2 = figure;
figure(fig_gold2)
plot(Aspect_ratios,drag_co,'b')
xlabel('Aspect Ratio');
foo1 = ylabel('C_d','Rotation',0);
set(foo1,'Units','Normalized','Position',[-0.1 0.55 0]);
%get another figure for line search
fig_line2 = figure;
figure(fig_line2);
plot(Aspect_ratios,drag_co,'b')
xlabel('Aspect Ratio');
foo2 = ylabel('C_d','Rotation',0);
set(foo2,'Units','Normalized','Position',[-0.1 0.55 0]);
%%%%%%%%%%%%%%%%%% LINESEARCH BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
x_prev = 1;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
%alpha_init = input('Enter inital step length: ');
alpha_max = 3;
hold on
plot(x_prev,this_obj(x_prev),'ks');
hold on
[f_init,g_init] = this_obj(x_prev);
%store each run of function for plotting convergence
sec_g_metric = [];
x_metric = x_prev;
counter = 1;
func_evals = 0;
%do a bad thing and have while loop to simulate do while
while counter < 100
	%compute the descent direction of the objective function
	%in this case we directly evaluate the gradient and then normalize
	[f_this,g_this] = this_obj(x_prev);
	p = -g_this./norm(g_this);
	step = mdoLineSearch(this_obj, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%update the function evaluations counter
	func_evals(end+1) = num_evals.count;
	%jump to this point
	x_curr = x_prev + p*step;
	x_metric(end+1) = x_curr;
	%these two never change
	hold on
	plot(x_curr,this_obj(x_curr),'rd');
	hold on
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = this_obj(x_curr);
	%exit condition is hidden just below
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
%%%%%%%%%%%%%%%%%% GOLDEN SECTION BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
a = 1;
b = 50;
epsilon = 1e-7;
%rezero the counter
num_evals.count = 0;

[left,right,sec_cost_data] = mdoGoldenSection(this_obj,a,b,epsilon);
disp(num_evals.count)
tolerance = (b-a)*1e-6; %visable convergence intervals
%figure out the relative error metric
gold_metric = abs(sec_cost_data(:,1) - sec_cost_data(:,3));
mdoPlotGoldenSectionData(sec_cost_data, fig_gold2, tolerance, 0.002) % using specialized plotting function
%plotting the function values
x_vals = a-.5:0.01:b+.5;
y_vals = zeros(length(x_vals),1);
for index = 1:length(x_vals)
	y_vals(index) = this_obj(x_vals(index));
end
plot(x_vals,y_vals)
hold on
plot(28.39424799779898,this_obj(28.39424799779898),'k*')

%compare the results from the line search
r_fig_metric = figure;
figure(r_fig_metric);
semilogy(2:length(sec_g_metric)+1, sec_g_metric,'gd:')
hold on

semilogy(1:length(x_metric), x_metric, 'ko-.')
hold on
semilogy(1:length(gold_metric), gold_metric, 'rd--')
hold on

%title('Convergence Metric')
xlabel('Iterations k')
legend('|g_k|/|g_0|','|x_k - x.|', '\delta_k');

%clean up the rest of the titles and figures
figure(fig_line2);
legend('C_d(A)','x_0','x_k')

figure(fig_gold2)
legend('C_d(A)')
