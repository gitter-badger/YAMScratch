function [drag_total] = mdoDragTotal(Aspect,Surface)
	validateattributes(Aspect,{'scalar'},{'positive'})
	validateattributes(Surface,{'scalar'},{'positive'})
	chord = sqrt(S/A);
	beam = sqrt(A*S);
	%%Constants
	W_0 = 4940;
	N_ult = 2.5;
	t_over_c = 0.12;

	disp(wingWeight(Surface, beam))
	

end

function [W_w] = wingWeight(Surface,beam)
	%Compute the wing weight using rearrangment of terms
	% W_w = W - W_0
	% W_w = 45.42*Surface + 8.71e-5 *(N_ult*b^3)/(S*(t/c))

	C_1 = 8.71e-5*N_ult*beam^3/(Surface*(t_over_c));
	C_2 = (W_0 + 45.42*Surface);
	W_w = roots([1, -(2*C_2+C_1^2*W_0), C_2^2]);
	return
end
