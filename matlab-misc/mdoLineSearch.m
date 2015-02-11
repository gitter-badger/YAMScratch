function [alpha] = mdoLineSearch(obj, p, x0, mu_1, mu_2, alpha_init, alpha_max)
% Purpose: returns a step length that satisfies the strong-Wolfe conditions
% Inputs:
% obj - a function handle for the objective that returns [f, f']
% p - the search direction; must be a descent direction for obj
% x0 - the location in the design space where the line search starts
% mu_1 - the sufficient decrease parameter; 0 < mu_1 < 1
% mu_2 - the curvature condition parameter; mu_1 < mu_2 < 1
% alpha_init - the initial step length
% alpha_max - the maximum allowable step length
% Outputs:
% alpha - a step satisfying the strong-Wolfe conditions

	%alpha_prev is a0 to start
	alpha_prev = 0;
	[f_0, g_0] = obj(x0);
	%keep a copy of the inital point evaluations around
	[f_prev, g_prev] = deal(f_0, g_0);
	alpha_this = alpha_init;
	index = 1;
	%this is only to pass the variables cleanly
	%use sensible defaults in case we have not calculated any of
	%these values by the time we pass the same struct into zoom stage
	bound_cond = struct;
	bound_cond.f_low = NaN; bound_cond.g_low = NaN;
	bound_cond.f_high = NaN; bound_cond.g_high = NaN;
	%these two below never change
	bound_cond.f_init = f_0; bound_cond.g_init = g_0;
	while true
		disp(index)
		%usually here we only evaluate the function
		%f_this = obj(x0 + p*alpha_this)
		%however here just do both together because it is so fast
		[f_this, g_this] = obj(x0 + p*alpha_this);
		%test the Amjiro condition right away
		if (f_this > (f_0 + mu_1*alpha_this*g_0)) ...
			|| ((f_this > f_prev) && (index > 1))
			%all of the pre calculated information is wrapped up in struct
			%and passed in as boundary condition for this interval
			bound_cond.f_low = f_prev; 
			%will not necessarily have the g_prev to assign to g_low at this point in code
			if (index > 1)
				bound_cond.g_low = g_prev;
			else
				bound_cond.g_low = NaN;
			end
			bound_cond.f_high = f_this; 
			bound_cond.g_high = g_this;
			alpha = mdoZoomStage(alpha_prev, alpha_this, bound_cond);
			return
		end
		%with expensive derivatives we would evaluate the derivative only if necessary
		%g_this = some derivative call here
		%below the statement is only valid for 1D case, need different formulation for N-D
		if (abs(g_this) <= -mu_2*g_0)
			alpha = alpha_this;
			return
		%statment also only valid for 1D case
		elseif (g_this >= 0)
			bound_cond.f_high = f_prev;
			bound_cond.g_high = g_prev;
			bound_cond.f_low = f_this; 
			bound_cond.g_low = g_this;
			alpha = mdoZoomStage(alpha_this,alpha_prev,bound_cond);
			return
		else
			%do a binary search for an acceptable step
			alpha_this = (alpha_this + alpha_max)/2;
			index = index + 1;
			%remember to swap over values so that subscripts work properly
			f_prev = f_this;
			g_prev = g_this;
		end
	end
end
