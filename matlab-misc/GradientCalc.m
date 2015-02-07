function [] = GradientCalc(fhandle,varargin)
	p = inputParser;
	defaultStep = ones(128,1);
	check_fun = @(x)(isa(x,'function_handle'));
	%load the requirements
	addRequired(p,'fhandle',check_fun);
	%run the parser on the inputs
	parse(p,fhandle,varargin{:});

	return
end

