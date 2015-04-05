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

X_0 = [20,30];
%===========================================================
%The plane object that holds all of the methods
plane1 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);
%===========================================================
%plot the contour and feasible region
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[AA,SS] = meshgrid(a,s);
Drag = arrayfun(@plane1.m_DragForce, AA, SS);
fig_1 = figure;
contour(AA,SS,Drag,50);
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
hold on

fun = @(aspect, surface)(plane1.m_LandingConstraint(aspect, surface, V_min, C_L_max));

% [scatA, scatS] = meshgrid([5:0.1:30],[5:0.1:40]);
% Constraint_out = arrayfun(fun, scatA, scatS);

% scatA(Constraint_out < 0) = NaN; 
% scatS(Constraint_out < 0) = NaN;
% scatter(scatA(:), scatS(:), 3, 'filled')

%============================================================
%           LOG BARRIER METHOD                              %
%============================================================
step = 1e-2;
[Close_A, Close_S] = meshgrid(13:step:14, 12:step:13);
[phi, grad_phi] = mdo.LogBarrierFactory(plane1, V_min, C_L_max);
temp_out = arrayfun(phi, Close_A, Close_S);
barrier_fig = figure;
contour(Close_A, Close_S, temp_out, 50)

%============================================================
%           FMINCON WITH SQP                                %
%============================================================
plane2 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);


lb = [1; 1];
ub= [inf; inf];
options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on');
fmin_obj = @(X)(plane2.m_DragForce(X(1), X(2)));
nonlincon = @(X)(plane2.m_nonLinearConstraint(X(1), X(2), V_min, C_L_max));
[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0,[], [], [], [], lb, ub, nonlincon, options );

hold on;
plot(x(1), x(2), 'kd');
hold on

