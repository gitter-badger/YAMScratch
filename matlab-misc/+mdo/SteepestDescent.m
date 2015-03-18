function [x_star, logObj] = SteepestDescent(linesearch, obj, grad, x0, logObj, tol)

	mu_1 = 1e-4;
	mu_2 = 0.9;
	max_iter = 500;
	alpha_init = 1;
	alpha_max = 1000000;
	obj0 = obj(x0);
	grad0 = grad(x0);
	%store the very first gradient in local memory
	grad_init = grad0;
	%start off
	xk = x0;
	%log the very first time
	iteration = 1;
	logObj.editIteration(iteration, obj0, xk, grad0, 1, 1)
	%do a bad thing and have infinite while loop to simulate do while
	while true
		pk = -grad0./norm(grad0);
		%note that we are converting to column vector
		[step, fnew, num_f_evals, num_df_evals] = linesearch(obj, grad, xk, pk, ...
		 	mu_1, mu_2, alpha_init, alpha_max, max_iter);
		%update for the next iteration and log
		iteration = iteration + 1;
		xk = xk + step*pk;
		grad0 = grad(xk);
		logObj.editIteration(iteration, fnew, xk, grad0, num_f_evals, num_df_evals+1);
		%check the gradient condition at this point
		if (norm(grad0) < norm(grad_init)*tol)
			break;
		end
	end
	x_star = xk;
	return
end
