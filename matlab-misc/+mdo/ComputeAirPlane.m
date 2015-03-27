classdef ComputeAirPlane < handle

	methods
		function obj = ComputeAirPlane()

		end
	end

	methods(Static)
		function [C_L] = CoefficeintLift(S, W, rho, Velocity)
			C_L = (2 * W) / (rho * Velocity^2 * Surface);
		end
		function [C_d] = CoefficeintDrag()

		end
		function [C_f] = CoefficeintFriction()

		end
		function [D] = DragForce(A,S)

		end
		function [] = LandingConstraint(X)
	end

end