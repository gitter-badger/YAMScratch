classdef ComputeAirPlane < handle
    properties
        N_ult;
        t_over_c;
        W_0;
        Rho;
        Mu;
        K;
        E;
        SWR;
        V;
    end
    methods
        function obj = ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity)
            obj.N_ult = N_ult;
            obj.t_over_c = t_over_c;
            obj.W_0 = W_0;
            obj.Rho = rho;
            obj.Mu = mu;
            obj.K = k;
            obj.E = e;
            obj.SWR = S_wet_ratio;
            obj.V = Velocity;
        end
    end

    methods(Static)
        function [C_L] = s_CoefficientLift(S, W, rho, Velocity)
            C_L = (2 * W) / (rho * Velocity^2 * S);
            return
        end
        function [grad_C_L] = s_gradCoefficientLift(S, W, del_W, Velocity, rho)
            assert(isvector(del_W))
            assert(length(del_W) == 2)
            k_1 = 2/(rho * Velocity^2);
            %get partial with respect to a
            %grad_C_L(1,1) = k_1 * (del_W(1)/S)
            %grad_C_L(2,1) = k_1 * (del_W(2)/S - (W / S^2))
            grad_C_L = k_1 * (del_W ./S - [0; W/S^2]);
            return
        end

        function [C_d] = s_CoefficientDrag(A, S, S_wet_ratio, k, e, C_L, C_f)
            C_d = (0.03062702/S) + k * C_f *S_wet_ratio + C_L ^2 / (pi*A*e);
            return
        end

        function [grad_C_d] = s_gradCoefficientDrag(A, S, S_wet_ratio, k, e, C_f, del_C_f, C_L, del_C_L)
            assert(isvector(del_C_f));
            assert(length(del_C_f) == 2);
            assert(isvector(del_C_L));
            assert(length(del_C_L) == 2);
            grad_C_d(1,1) = 0 + (k * S_wet_ratio * del_C_f(1))  + ( (2* C_L* del_C_L(1) / A ) - (C_L^2/A^2)) / (pi* e)
            grad_C_d(2,1) = (-0.03062702/S^2) + (k*S_wet_ratio*del_C_f(2)) + (1/(pi*e*A)) * (2* C_L * del_C_L(2))
            %grad_C_d = [0 ; -0.03062702/S^2]  + (k * S_wet_ratio * del_C_f) + (1/(pi*e)) * ((2*C_L * del_C_L / A) + [-C_L^2/A^2; 0]);
            return
        end

        function [C_f] = s_CoefficientFriction(S, rho, Velocity, mu)
            C_f = 0.074 * (rho*Velocity*sqrt(S)/mu)^-0.2;
            return
        end

        function [grad_C_f] = s_gradCoefficientFriction(S, rho, Velocity, mu)
            %does not depend on A
            grad_C_f(1,1) = 0;
            grad_C_f(2,1) = 0.074 * ((rho * Velocity)/mu)^(-0.2) * (-1/10) * S^(-11/10);
            return
        end

        function [valid_weight] = s_WingWeight(A, S, W_0, N_ult, t_over_c)
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

        function [grad] = s_gradWingWeight(A, S, W_0, N_ult, t_over_c, varargin)
            %first compute the weight
            if nargin == 5
                W = W_0 + mdo.ComputeAirPlane.s_WingWeight(A, S, W_0, N_ult, t_over_c);
            else
                %get the cached value of the wing total weight
                W = varargin{1};
            end

            k_1 = 45.42;
            k_2 = (8.71e-5 * N_ult * sqrt(W_0)) / t_over_c;

            k_3 = k_2 * S^(1/2);
            top_part_A = k_3 * ((3/2) * A^(1/2) * W^(1/2));
            bot_part_A = 1 - k_3 * (A^(3/2) * (1/2) * W^(-1/2));
            grad(1,1) = top_part_A / bot_part_A;

            k_4 = k_2 * A^(3/2);
            top_part_S = k_1 +  k_4* ((1/2) * S^(-1/2) * W^(1/2));
            bot_part_S = 1 - k_4 * (S^(1/2) * (1/2) * W^(-1/2));
            grad(2,1) = top_part_S / bot_part_S;
        end
    end
    %member methods
    methods
        function [Drag, gradient] = DragForce(self, X)
            assert(isvector(X))
            A = X(1);
            S = X(2);
            %compute the weight of the wing
            W = obj.W_0 + obj.wingWeight(A, S, obj.W_0, obj.N_ult, obj.t_over_c);

            if naragout == 2
                %capture the gradients

            elseif naragout == 1

                %Coeff_L = obj.CoefficientLift(S, W)
            end
            
            return
        end

        function [val, gradient] = LandingConstraint(A, S, V_min, C_l_max)

            if naragout == 2

            end
            return
        end
    end
end

