function [fhandle] = IterationLogger(logObj)

	function [stop] = ActualIterationLogger(x, optimValues, state)
		stop = false;
		%optimValues
		k = optimValues.iteration + 1;
		logObj.editIteration(k, optimValues.fval, x , optimValues.gradient, optimValues.funccount, 0, optimValues.firstorderopt);

		%, optimValues.firstorderopt
	end
	fhandle = @ActualIterationLogger;
	return
end