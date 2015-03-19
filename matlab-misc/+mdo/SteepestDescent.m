function [x_star, logObj] = SteepestDescent(linesearch, obj, grad, x0, logObj, ls_parameters)

	mu_1 = ls_parameters(1);
	mu_2 = ls_parameters(2);
	max_iter = 500;
	alpha_init = ls_parameters(3);
	alpha_max = ls_parameters(4);
	tol = ls_parameters(5);

	obj0 = obj(x0);
	gk = grad(x0);
	%store the very first gradient in local memory
	grad_init = gk;
	%start off
	xk = x0;
	%log the very first time
	iteration = 1;
	logObj.editIteration(iteration, obj0, xk, gk, 1, 1)
	%do a bad thing and have infinite while loop to simulate do while
	while true
		pk = -gk;
		%note that we are converting to column vector
		[step, fnew, num_f_evals, num_df_evals] = linesearch(obj, grad, xk, pk, ...
		 	mu_1, mu_2, (alpha_init / (2*norm(pk))) , (alpha_max / norm(pk)), max_iter);
		%update for the next iteration and log
		iteration = iteration + 1;
		xk = xk + step*pk;
		gk = grad(xk);
		logObj.editIteration(iteration, fnew, xk, gk, num_f_evals, num_df_evals+1);
		%check the gradient condition at this point
		if (norm(gk) < norm(grad_init)*tol)
			break;
		end
	end
	x_star = xk;
	return
end
