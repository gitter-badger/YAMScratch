function [drag_total] = DragTotal(Aspect,Surface)
	validateattributes(Aspect,{'scalar','double'},{'positive'})
	validateattributes(Surface,{'scalar','double'},{'positive'})
	chord = sqrt(Surface/Aspect);
	beam = sqrt(Aspect*Surface);
	%%Constants
	W_0 = 4940;
	N_ult = 2.5;
	t_over_c = 0.12;
	function [W_w] = wingWeight()
		%Compute the wing weight using rearrangment of terms
		% W_w = W - W_0
		% W_w = 45.42*Surface + 8.71e-5 *(N_ult*b^3)/(S*(t/c))

		C_1 = 8.71e-5*N_ult*beam^3/(Surface*(t_over_c));
		C_2 = (W_0 + 45.42*Surface);
		W_w = roots([1, -(2*C_2+C_1^2*W_0), C_2^2]) - W_0;
		return
	end

	function [result] = validateWingWeight(test)
		validateattributes(Aspect,{'scalar','double'},{'positive'})
		W_total = W_0 + test;
		W_weight = 45.42*Surface + 8.71e-5*N_ult*beam^3*sqrt(W_0*W_total)/(Surface*t_over_c);
		if abs(W_weight - test) < 1e-9;
			result = true;
		else
			result = false;
		end
		return
	end

	foo = wingWeight();
	disp(foo)
	validateWingWeight(foo(1))
	validateWingWeight(foo(2))
end
