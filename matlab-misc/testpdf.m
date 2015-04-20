function [result] = testpdf(x, mu, varargin)
	disp([mu, lambda])
	test_x = x-mu;
	result = zeros(size(x));
	for index = 1:length(x)
		result(index) = exppdf(test_x(index), varargin{1}, varargin{2}, varargin{3});
	end
end
