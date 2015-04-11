classdef ComputeAirPlane2 < mdo.ComputeAirPlane

    methods
        function obj = ComputeAirPlane2(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity)
            obj@mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity)
        end
    end
    
    methods(Static)
        function [C_f] = s_CoefficientFriction(A, S, rho, Velocity, mu)
            chord = sqrt(S/A);
            if chord > 0.85
                C_f = 0.074 * (rho*Velocity*chord/mu)^-0.2;
            else
                C_f = 1.328 * (rho*Velocity*chord/mu)^-0.5;
            end
            return
        end

        function [grad_C_f] = s_gradCoefficientFriction(A, S, rho, Velocity, mu)
            %does not depend on A
            if sqrt(S/A) > 0.85
                grad_C_f(1,1) = 0.074 * ((rho * Velocity * sqrt(S))/mu)^(-0.2) * (1/10) * A ^(-9/10);
                grad_C_f(2,1) = 0.074 * ((rho * Velocity* sqrt(1/A))/mu)^(-0.2) * (-1/10) * S^(-11/10);
            else
                grad_C_f(1,1) = 1.328 * ((rho * Velocity * sqrt(S))/mu)^(-0.5) * (0.25) * A ^(-3/4);
                grad_C_f(2,1) = 1.328 * ((rho * Velocity * sqrt(1/A))/ mu)^(-0.5) * (-0.25) * S^(-5/4);
            end
            return
        end
    end
end
