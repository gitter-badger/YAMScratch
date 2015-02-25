function [f_prime] = GradientCalc(fhandle,varargin)
	p = inputParser;
	VECTOR_SIZE = 128; 
	defaultStep = 37;

	defaultLocation = zeros(VECTOR_SIZE,1);
	wrapper = @(M)(validSize(M,VECTOR_SIZE));
	check_fun = @(x)(isa(x,'function_handle'));
	%load the requirements
	addRequired(p,'fhandle',check_fun);
	addOptional(p,'step',defaultStep,wrapper);
	addOptional(p,'location',defaultLocation,wrapper);
	%run the parser on the inputs
	parse(p,fhandle,varargin{:});
	if p.Results.step == 37
		STEP = [1:VECTOR_SIZE];
		for dimension = 1:VECTOR_SIZE
			STEP(dimension) = 1e-7;
			%STEP(dimension) = max(1,p.Results.location(dimension))*sqrt(eps(p.Results.location(dimension)));
		end
	end
	disp(p.Results)
	LOCATION = p.Results.location;
	%evaluate the function at the location
	f_0 = p.Results.fhandle(p.Results.location);
	%preallocate for speed
	f_steps = zeros(VECTOR_SIZE,1);
	f_prime = zeros(VECTOR_SIZE,1);
	%loop over all dimensions and compute partial
	for dimension = 1:VECTOR_SIZE
		%store the scalar in the dimension
		disp(dimension)
		swap = LOCATION(dimension);
		LOCATION(dimension) = LOCATION(dimension) + STEP(dimension);
		f_steps(dimension) = p.Results.fhandle(LOCATION);
		f_prime(dimension) = f_steps(dimension) - f_0 / STEP(dimension);
		%put it back
		LOCATION(dimension) = swap;
	end
	return
end

function [result] = validSize(Matrix,vec_size)
		[r,c] = size(Matrix);
		result = (r == vec_size || c == vec_size ) && isvector(Matrix);
end
