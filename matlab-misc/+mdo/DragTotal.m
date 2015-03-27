function [drag_total] = DragTotal(Aspect,Surface)
	%no need to extract the real part, sqrt works for imaginary
	assert(Aspect >= 0);
	assert(Surface >= 0);
	chord = sqrt(Surface / Aspect);
	beam = sqrt(Aspect * Surface);
	P = mdo.AirPlane(Aspect, Surface);
	%note that the actual size of the airplane is not used,
	%just its properties
	W_0 = 4940;
	N_ult = 2.5;
	t_over_c = 0.12;
	W_w = wingWeight();
	wing_total = W_w + W_0;

	Coeff_L = 2 * wing_total / (P.rho * P.Velocity^2 * Surface);
 
	Coeff_f = 0.074 * (P.rho*P.Velocity*sqrt(Surface)/P.mu)^-0.2;

	Coeff_D = (0.03062702 / Surface) ...
			+ (P.k * Coeff_f * (P.S_wet_ratio)) ...
			+ Coeff_L^2/(pi*Aspect*P.e);

	drag_total = wing_total * Coeff_D/ Coeff_L;

	function [wing_weight] = wingWeight()
		%Compute the wing weight using rearrangment of terms
		% W_w = W - W_0
		% W_w = 45.42*Surface + 8.71e-5 *(N_ult*b^3)/(S*(t/c))
		C_1 = 8.71e-5*N_ult*beam^3/(Surface*(t_over_c));
		C_2 = (W_0 + 45.42*Surface);
		candiates = roots([1, -(2*C_2+C_1^2*W_0), C_2^2]) - W_0;
		wing_weight = validateWingWeight(candiates);
		return
	end

	function [valid_weight] = validateWingWeight(test_vec)
		W_total = W_0*ones(2,1) + test_vec;
		wing_weight = 45.42*Surface + 8.71e-5*N_ult*beam^3*sqrt(W_0.*W_total)/(Surface*t_over_c);
		result = (abs(wing_weight - test_vec) < 1e-9);
		if result(1)
			valid_weight = test_vec(1);
		elseif result(2)
			valid_weight = test_vec(2);
		else
			ME = MException('InvalidWeight:OutofBounds','Check not satisfied');
			throw(ME)
		end
			return
	end
	return
end
