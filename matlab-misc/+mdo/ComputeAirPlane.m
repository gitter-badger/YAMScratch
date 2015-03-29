classdef ComputeAirPlane < handle

	methods
		function obj = ComputeAirPlane()

		end
	end

	methods(Static)
		function [C_L] = CoefficientLift(S, W, rho, Velocity)
			C_L = (2 * W) / (rho * Velocity^2 * Surface);
		end
		function [C_d] = CoefficientDrag()

		end
		function [C_f] = CoefficientFriction()

		end
		function [D] = DragForce(A,S)

		end
		function [val] = LandingConstraint(X)

		end

		function [valid_weight] = wingWeight(A, S, W_0, N_ult, t_over_c)
			%Compute the wing weight using rearrangment of terms
			% W_w = W - W_0
			% W_w = 45.42*Surface + 8.71e-5 *(N_ult*b^3)/(S*(t/c))
			beam = sqrt(A * S);

			C_1 = 8.71e-5*N_ult*beam^3/(S*(t_over_c));
			C_2 = (W_0 + 45.42*S);
			candiates = roots([1, -(2*C_2+C_1^2*W_0), C_2^2]) - W_0;
			
			%now check the roots and find the valid weight
			W_total = W_0*ones(2,1) + candiates;
			wing_weight = 45.42*S + 8.71e-5*N_ult*beam^3*sqrt(W_0.*W_total)/(S*t_over_c);
			result = (abs(wing_weight - candiates) < 1e-9);
			if result(1)
				valid_weight = candiates(1);
			elseif result(2)
				valid_weight = candiates(2);
			else
				ME = MException('InvalidWeight:OutofBounds','Check not satisfied');
				throw(ME)
			end
			return
		end

		function [grad] = gradientWingWeight(A, S, W_0, N_ult, t_over_c, varargin)
			%first compute the weight
			if nargin == 5
				W = W_0 + mdo.ComputeAirPlane.wingWeight(A, S, W_0, N_ult, t_over_c)
			else
				%get the cached value of the wing total weight
				W = varargin{1};
			end

			k_1 = 45.42;
			k_2 = 8.71e-5 * N_ult * sqrt(W_0) / t_over_c;

			k_3 = k_2 * S^(-5/2);
			top_part_A = k_3 * ((3/2) * A^(1/2) * W^(1/2));
			bot_part_A = 1 - k_3 * (A^(3/2) * (1/2) * W^(-1/2));
			grad(1,1) = top_part_A / bot_part_A;

			k_4 = k_2 * A^(3/2);
			top_part_S = k_1 +  k_4* ((-5/2) * S^(-7/2) * W^(1/2));
			bot_part_S = 1 - k_4 * (S^(-5/2) * (1/2) * W^(-1/2));
			grad(2,1) = top_part_S / bot_part_S;
		end
	end
end

