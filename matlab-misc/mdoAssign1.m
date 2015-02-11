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
x0 = -1;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = input('Enter inital step length: ');
alpha_max = 3;
%compute the descent direction of the objective function
%in this case we directly evaluate the gradient and then normalize
[dummy,g] = objective(x0);
p = -g./norm(g);
step = mdoLineSearch(objective, p, x0, mu_1, mu_2, alpha_init, alpha_max);
%jump to this point
x1 = x0 + step;
hold on
plot(x1,simple(x1),'rd');
