%Script to run the line search on the simple test platform
%Copyright (c) 2015 Isaiah Bell
%All rights reserved
clear
clc
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
x_prev = -1;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = input('Enter inital step length: ');
alpha_max = 3;
%compute the descent direction of the objective function
%in this case we directly evaluate the gradient and then normalize
[f_init,g_init] = objective(x0);
p = -g_init./norm(g_init);
%do a bad thing and have infinite while loop to simulate do while
while true
	step = mdoLineSearch(objective, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%jump to this point
	x_curr = x_prev + step;
	%these two never change
	hold on
	plot(x1,simple(x1),'rd');
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = objective(x1);
	if (norm(g_curr) < norm(g_init)*1e-6)
		break;
	end
end
