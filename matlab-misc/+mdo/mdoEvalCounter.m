function [fhandle] = mdoEvalCounter(func_handle,counter)
	counter.count = 0;
	function varargout = wrapper(varargin)
		counter.increment();
		%only accept wrapped functions with constant number of outputs
		num_out = nargout(func_handle);
		baseException = MException('wrapper:notCool','foo');
		if num_out < 0
			throw(baseException)
		end
		temp = zeros(num_out,1);
		[varargout{1:num_out}] = func_handle(varargin{:});
		return
	end
	fhandle = @wrapper;
	return
end
