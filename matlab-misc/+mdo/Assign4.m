clear;
close all
import mdo.*

A = 13;
S = 27;
%compute the gradient of the wing weight function
N_ult = 2.5;
t_over_c = 0.12;
W_0 = 4940;
cs_h = 1e-20

W_w = mdo.ComputeAirPlane.wingWeight(A, S, W_0, N_ult, t_over_c)

dWdA = imag(mdo.ComputeAirPlane.wingWeight((A + 1i * cs_h), S, W_0, N_ult, t_over_c)) / cs_h;
dWdS = imag(mdo.ComputeAirPlane.wingWeight(A , (S+ 1i * cs_h), W_0, N_ult, t_over_c)) / cs_h;

grad = mdo.ComputeAirPlane.gradientWingWeight(A, S, W_0, N_ult, t_over_c);

options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on');
