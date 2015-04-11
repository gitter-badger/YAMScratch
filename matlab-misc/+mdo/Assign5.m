clear;
close all
import mdo.*

DEBUG = false;

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

V_min = 22;         %maximum landing velocity
C_L_max = 2;        %maximum coefficient of lift at landing

X_0 = [10; 35];
%===========================================================
%The plane object that holds all of the methods
plane = mdo.ComputeAirPlane2(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);
a = [5:1e-1:30]; % A
s = [5:1e-1:40]; % S
[AA,SS] = meshgrid(a,s);
Drag = arrayfun(@plane.m_DragForce, AA, SS);
fig_1 = figure;
contour(AA,SS,Drag,50);
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
hold on
plot(a,a*0.85^2,'r--')

dCfdA = imag(plane.s_CoefficientFriction(15 + 1i * 1e-20, 5, rho, Velocity, mu))/1e-20;
dCfdS = imag(plane.s_CoefficientFriction(15, 5 + 1i * 1e-20, rho, Velocity, mu))/1e-20;

del_cf = plane.s_gradCoefficientFriction(15, 5, rho, Velocity, mu);

del_cf - [dCfdA;  dCfdS];
