function [grad] = ComplexStepGrad(obj, x, cs_h)
%Purpose: return the gradient of an objective function using the 
%		  complex step method
%Inputs:
%	obj - a function handle to the objective function
%	x - input vector
%	cs_h - the complex step size
%Ouputs:
%	grad - a vector of the same size as x, so if x is a column
%			column vector then the gradient is the same.
	assert(isvector(x));
	[r,c] = size(x);
	N = max(r,c);
	%make an empty vector to store result of gradient calc
	grad = zeros(1, N);
	for index = 1:N
		temp_x = x;
		temp_x(index) = temp_x(index) + 1i*cs_h;
		grad(1,index) = imag(obj(temp_x));
	end
	grad = grad ./ cs_h;
	if r == N
		%then we want to transpose the result
		grad = grad.';
	end
	return;
end
