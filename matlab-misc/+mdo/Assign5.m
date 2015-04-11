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

tic
Drag = arrayfun(@plane.m_DragForce, AA, SS);
toc

fig_1 = figure;
contour(AA,SS,Drag,50);
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
hold on

disconline = a*0.85^2;
disconline(disconline < 5) = NaN;
plot(a,disconline,'r--')
%===========================================================
obj = @(X)(plane.m_DragForce(X(1), X(2)));
x_star = Swarm1(obj, [5;5], [40;40]);

plot(x_star(1), x_star(2), 'kd')
