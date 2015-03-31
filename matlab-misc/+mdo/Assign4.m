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

V_min = 22;         %maximum landing velocity
C_L_max = 2;        %maximum coefficient of lift at landing


plane = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);



a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[AA,SS] = meshgrid(a,s);
Drag = arrayfun(@plane.m_DragForce, AA, SS);
fig_1 = figure;
contour(AA,SS,Drag,50);
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
hold on

fun = @(aspect, surface)(plane.m_LandingConstraint(aspect, surface, V_min, C_L_max));

Constraint_out = arrayfun(fun, AA, SS);

AA(Constraint_out < 0) = NaN; 
SS(Constraint_out < 0) = NaN;
plot(AA(:), SS(:), 'r*')

X_0 = [20,30]
lb = [1; 1]
ub= [inf; inf]
options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on');

fmin_obj = @(X)(plane.m_DragForce(X(1), X(2)));

nonlincon = @(X)(plane.m_nonLinearConstraint(X(1), X(2), V_min, C_L_max););


[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0,[], [], [], [], lb, ub, nonlincon, options );
