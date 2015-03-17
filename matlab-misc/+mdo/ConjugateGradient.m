%% ConjugateGradient: function description
function [x_star, history] = ConjugateGradient(obj, grad, LineSearch, x_0, e_g, e_a, e_r)
%Purpose: returns the location of a local minimum and a history object
%Inputs:
%	obj - a function handle for the objective
%	grad - a function handle for the objective gradient
%	LineSearch - a function handle to a line search implementation to specific interface
%	x_0 - starting point, is a vector
%	e_g - epsilon tolerance for the gradient
% 	e_a - epsilon tolerance for the absolute
%	e_r - epsilon tolerance for the scale factor

	k = 0;
	x_prev = x_0;
	x_curr = x_0;
	%start out by computing the steepest descent direction
	
	p_curr
	while True
		%we skip this round on the begining
		if k ~= 0
			B = (g_curr.' * g_curr)/(g_prev.' * g_prev);
			p_curr = -g_curr + B*p_prev;
		end
		%perform a line search
		step = LineSearch
		%add the information to the history object

		%update the memoized data
		p_prev = p_curr;
		x_prev = x_curr;
		g_prev = g_curr;

		if abs(f_curr - f_prev) <= (e_a + e_r*abs(f_prev) and norm(g_prev) <= e_g
			break
		end
	end

    x_star = 0; 
    return
end
