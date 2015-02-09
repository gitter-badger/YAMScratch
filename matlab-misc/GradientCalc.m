function [] = GradientCalc(fhandle,varargin)
	p = inputParser;
	VECTOR_SIZE = 128; 
	defaultStep = ones(VECTOR_SIZE,1);
	defaultLocation = zeros(VECTOR_SIZE,1);
	 
	check_fun = @(x)(isa(x,'function_handle'));
	function [result] = validSize(Matrix)
		[r,c] = size(Matrix);
		result = (r == VECTOR_SIZE || c == VECTOR_SIZE ) && isvector(Matrix);
	end
	%load the requirements
	addRequired(p,'fhandle',check_fun);
	addOptional(p,'step',defaultStep,validSize);
	addOptional(p,'location',defaultLocation,validSize);
	%run the parser on the inputs
	parse(p,fhandle,varargin{:});
	disp(p.Results)

	return
end
