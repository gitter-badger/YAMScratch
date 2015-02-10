function [alpha] = linesearch(obj, p, x0, mu_1, mu_2, alpha_max)
% Purpose: returns a step length that satisfies the strong-Wolfe conditions
% Inputs:
% obj - a function handle for the objective that returns [f, f']
% p - the search direction; must be a descent direction for obj
% x0 - the location in the design space where the line search starts
% mu_1 - the sufficient decrease parameter; 0 < mu_1 < 1
% mu_2 - the curvature condition parameter; mu_1 < mu_2 < 1
% alpha_max - the maximum allowable step length
% Outputs:
% alpha - a step satisfying the strong-Wolfe conditions