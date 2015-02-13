function [fhandle] = mdoEvalCounter(func_handle,counter)
	counter.count = 37
	function varargout = wrapper(varargin)
		counter.increment();
		%only accept wrapped functions with constant number of outputs
		num_out = nargout(func_handle);
		disp(num_out)
		baseException = MException('wrapper:notCool','foo');
		if num_out < 0
			%throw(baseException)
		end
		temp = zeros(num_out,1);
		[varargout{:}] = deal(func_handle(varargin{:}));
		return
	end
	fhandle = @wrapper;
	return
end
