function [x_star, logObj] = ConjugateGradient(linesearch, obj, grad, x0, e_g, e_a, e_r, logObj, ls_parameters)
%Purpose: returns the location of a local minimum and a history object
%Inputs:
%	obj - a function handle for the objective
%	grad - a function handle for the objective gradient
%	LineSearch - a function handle to a line search implementation with a specific interface
%	x_0 - starting point, is a vector
%	e_g - epsilon tolerance for the gradient
% 	e_a - epsilon tolerance for the absolute
%	e_r - epsilon tolerance for the scale factor
%	logObj - a MajorInterationHistory object to record each iteration
%	ls_parameters - five vector containing line search weights
%Ouputs:
%	x_star - best guess of local minimum meeting the convergence criteria passed in
%	logObj - used to return the logging object in case a handle subclass is not used
	
	%sanity checks
	assert(isvector(x0));
	assert(isvector(ls_parameters));

	mu_1 = ls_parameters(1);
	mu_2 = ls_parameters(2);
	max_iter = 500;
	alpha_init = ls_parameters(3);
	alpha_max = ls_parameters(4);
	tol = ls_parameters(5);
	f_prev = obj(x0);
	gk = grad(x0);
	%store the very first gradient in local memory
	grad_init = gk;
	%start off
	xk = x0;
	%log the very first time
	k = 1;
	logObj.editIteration(k, f_prev, xk, gk, 1, 1);
	x_prev = x0;
	
	%figure out the size of the input vector to determine how often to restart
	[r,c] = size(x0);
	N = max(r,c);
	p_prev = NaN;

	while true
		%we reset every N iterations using steepest descent
		if mod(k, N) == 1
			%start out by computing the steepest descent direction
			pk = -gk;
		else
			B = (gk.' * gk)/(g_prev.' * g_prev);
			pk = -gk + B*p_prev;
		end
		%perform a line search
		[step, fnew, num_f_evals, num_df_evals] = linesearch(obj, grad, xk, pk, ...
		 	mu_1, mu_2, (1/(norm(pk))), (alpha_max * (1/norm(pk))), max_iter);
		%update for next iteration and log
		k = k + 1;
		p_prev = pk;
		x_prev = xk;
		g_prev = gk;
		%update the x position and gradient
		xk = xk + step*pk;
		gk = grad(xk);

		logObj.editIteration(k, fnew, xk, gk, num_f_evals, num_df_evals+1);

		%sprintf('Loop iteration %d', k)
		%This condition from the slide and scales input
			% deltaf = abs(fnew - f_prev);
			% if deltaf <= (e_a + e_r * abs(f_prev))
			% 	if norm(g_prev) <= e_g
			% 		r = true;
			% 	else
			% 		r = false;
			% 	end

			% else 
			% 	r = false;
			% end
		%This condition only relies on the gradient
		if (norm(gk) < tol * norm(grad_init))
			r = true;
		else
			r = false;
		end

		if r
			break;
		end
		%update the previous f value after we use it
		f_prev = fnew;
	end
    x_star = xk; 
    return
end
