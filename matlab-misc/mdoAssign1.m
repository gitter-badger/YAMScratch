%Script to run the line search on the simple test platform
%Copyright (c) 2015 Isaiah Bell
%All rights reserved
clear
clc
%test function
simple = @(x) (x.^2-x);
%plotting the function
test_x = [-2:0.01:2];
simple_y = simple(test_x);
plot(test_x,simple_y)
%create an objective function handle to pass around
objective = @(x)(deal((x^2-x),(2*x-1)))
