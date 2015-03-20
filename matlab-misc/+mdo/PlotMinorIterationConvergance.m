function PlotMinorIterationConvergance(fig, logObj, format_spec, varargin)
%Purpose: takes a figure handle to append to and plots the gradient convergence metric
%Inputs:
%	fig - figure handle
%	logObj - a MajorIterationHistory object that has stored the intermediate steps of
%			an iterative optimization algorithm
%	format_spec - a string used to control what the data series looks like, passed into plot
%	varargin{1} - a handle to a specific subplot
%Outputs:
%	No return value but has side effects.
	figure(fig)
	hold on
	%check if we are just appending
	if nargin == 4
		subplot(varargin{1})
	end

	g_init_norm = norm(logObj.g(1,:));
	g_metric = ones(1, logObj.total_iterations);
	for index = 1:logObj.total_iterations
		g_metric(index) = norm(logObj.g(index,:)) / g_init_norm;
	end
	func_evals = ones(1, logObj.total_iterations);
	func_evals = logObj.fevals(1);
	for index = 2:logObj.total_iterations
		func_evals(index) = func_evals(index-1) + logObj.fevals(index);
	end
	semilogy(func_evals, g_metric, format_spec);
	return;
end