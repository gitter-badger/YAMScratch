function [left,right,cost_data] = mdoGoldenSection(obj,a,b,epsilon)
% Purpose: returns the location of a point that is in the neighborhood of a stationary point
% Inputs:
% obj - function handle which returns [f,f']
% a - the left side of 1D interval
% b - the right side of 1D interval
% epsilon - the tolerance criteria for neighborhood
% should statisfy a < b 
% Outputs:
% left - left side of interval containing minimum
% right - right side of interval containing minimum
% cost_data - history of points visited for plotting purposes
	%using these at floating point is not great but does work for this toy example
	tau = (sqrt(5) - 1)/2;
	%find the original endpoints
	f_a = obj(a);
	f_b = obj(b);
	c = NaN;
	d = NaN;
	iteration = 1;
	%fake a do while loop with the condition at end
	while true
		%only do logging if we are going to use it
		if nargout == 2
			cost_data(iteration,1) = a; cost_data(iteration,2) = f_a; 
			cost_data(iteration,3) = b; cost_data(iteration,4) = f_b;
		end
		iteration = iteration + 1;
		%only update the one which has not been calculated
		if c == NaN
			c = b + tau*(a-b)
			f_c = obj(c);
		end
		if d == NaN
			d = a + tau*(b-a)
			f_d = obj(d);
		end
		%store the values of c and d for termination condition
		x_2 = c;
		x_4 = d;
		%compare the values
		if f_c < f_d
			%then the interval [a,d] contains min
			%move the end of interval so that b is now where d is
			b = d;
			f_b = f_d;
			%store the result of c as the closer to b, point d
			d = c;
			f_d = f_c
			%set the flag so that point c will be recalculated
			c = NaN;
			f_c = NaN;
		else
			%then the other interval [c,b] contains the min
			a = c;
			f_a = f_c;
			%store the result of d as the closer to a, point c
			c = d;
			f_c = f_d;
			%set the flag so that point d will be recalculated
			d = NaN;
			f_d = NaN;
		end
		%relative magnitude  condition
		%checks the bracket size relative to the central value
		%this relative error is close to square of error in f(x)
		if (abs(b-a) < epsilon*(abs(x_2)+ abs(x_4)))
			break
		end
	end
	return
end
