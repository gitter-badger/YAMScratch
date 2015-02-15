clear('all');
clc
close all;
%configure the linesearch parameters here
x_prev = ones(128,1);
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = .9;
alpha_max = 3;
%%%%%%%%%%%%%%%%%% LINESEARCH BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[f_init,g_init] = func3(x_prev);
%store each run of function for plotting convergence
g_metric = [];
x_metric = [x_prev];
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
