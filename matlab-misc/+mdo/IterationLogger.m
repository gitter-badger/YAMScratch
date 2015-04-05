function [fhandle] = IterationLogger(logObj)

	function [stop] = ActualIterationLogger(x, optimValues, state)
		stop = false;
		%optimValues
		%logObj.editIteration(optimValues.iteration, optimValues, , 0, 0, 0)
	end
	fhandle = @ActualIterationLogger;
	return
end