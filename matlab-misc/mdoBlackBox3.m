clear('all');
clc
close all;
%configure the linesearch parameters here
x_prev = ones(128,1)*-5;
x_prev(37) = 3;
x_prev(75) = 0;
x_prev(13) = -1;
x_prev(25) = 2.6;
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
alpha_max = 3;
num_evals = mdoCounter;
obj = mdoEvalCounter(@func3,num_evals);
%%%%%%%%%%%%%%%%%% LINESEARCH BEGIN %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[f_init,g_init] = obj(x_prev);
%store each run of function for plotting convergence
g_metric = [];
x_metric(1,:) = [x_prev];
%do a bad thing and have infinite while loop to simulate do while
[f_curr,g_curr] = deal(f_init,g_init);
format_grad_norm = '\t[f, norm(g)] = %10.5g , %10.5g \n';
format_func_evals = '\tTotal function evaluations: %d \n';
iteration = 1;
func_cost = [];
while true
	%compute the descent direction of the objective function
	%in this case we directly evaluate the gradient and then normalize
	fprintf('Iteration: %d\n',iteration );
	fprintf(format_grad_norm, f_curr, norm(g_curr));
	output_count = num_evals.count;
	fprintf(format_func_evals,output_count );
	func_cost(end+1) = output_count;
	p = -g_curr./norm(g_curr);
	step = mdoLineSearch(obj, p, x_prev, mu_1, mu_2, alpha_init, alpha_max);
	%jump to this point
	x_curr = x_prev + p*step;
	x_metric(end+1,:) = x_curr;
	%test if the gradient has been reduced enough to quit
	[f_curr, g_curr] = obj(x_curr);
	if (norm(g_curr) < norm(g_init)*1e-15)
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
		break;
	else
		g_metric(end+1) = (norm(g_curr)/norm(g_init));
	end
	x_prev = x_curr;
	iteration = iteration + 1;
end
