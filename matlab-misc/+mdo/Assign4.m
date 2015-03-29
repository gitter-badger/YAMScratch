clear;
close all
import mdo.*

%Define Constants
N_ult = 2.5;
t_over_c = 0.12;
W_0 = 4940;
rho = 1.23;         %density of air
mu = 17.8e-6;       %viscosity of air
k = 1.2;            %form factor
e = 0.96;           %Oswald efficiency factor
S_wet_ratio = 2.05;
Velocity = 35;

V_max = 22;         %maximum landing velocity
C_L_max = 2;        %maximum coefficient of lift at landing

%test the Coefficeint of lift
A = 13;
S = 27;
cs_h = 1e-20;
plane = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);

We = plane.s_WingWeight(A, S, plane.W_0, plane.N_ult, plane.t_over_c);
del_W = plane.s_gradWingWeight(A, S, plane.W_0, plane.N_ult, plane.t_over_c, We);
disp('A====================')
dCLdA = imag(plane.s_CoefficientLift(S, A + 1i*cs_h, plane.Rho, plane.V, plane.W_0, plane.N_ult, plane.t_over_c )) /cs_h;
disp('S====================')
dCLdS = imag(plane.s_CoefficientLift(S + 1i*cs_h, A ,plane.Rho, plane.V, plane.W_0, plane.N_ult, plane.t_over_c )) /cs_h;

grad_CL = plane.s_gradCoefficientLift(S, We, del_W, Velocity, rho)

options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on');



fmin_obj = @plane.DragForce;

nonlcon = @mdo.LandingConstraint;
