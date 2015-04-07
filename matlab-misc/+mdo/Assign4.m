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

X_0 = [20; 30];
%===========================================================
%The plane object that holds all of the methods
plane7 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);
%===========================================================
%plot the contour and feasible region
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[AA,SS] = meshgrid(a,s);
Drag = arrayfun(@plane7.m_DragForce, AA, SS);
fig_1 = figure;
contour(AA,SS,Drag,50);
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
hold on

fun = @(aspect, surface)(plane7.m_LandingConstraint(aspect, surface, V_min, C_L_max));

% [scatA, scatS] = meshgrid([5:0.1:30],[5:0.1:40]);
% Constraint_out = arrayfun(fun, scatA, scatS);

% scatA(Constraint_out < 0) = NaN; 
% scatS(Constraint_out < 0) = NaN;
% scatter(scatA(:), scatS(:), 3, 'filled')

%============================================================
%           LOG BARRIER METHOD                              %
%============================================================
% step = 1e-2;
% [Close_A, Close_S] = meshgrid(13:step:14, 12:step:13);
% [phi, grad_phi] = mdo.LogBarrierFactory(plane7, V_min, C_L_max);
% temp_out = arrayfun(phi, Close_A, Close_S);
% barrier_fig = figure;
% contour(Close_A, Close_S, temp_out, 50);


plane1 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = .5;
alpha_max = 10;
tolerance = 1e-2;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
e_g = 1e-6;
e_a = 1e-6;
e_r = 1e-6;

[obj, grad] = mdo.LogBarrierFactory(plane1, V_min, C_L_max);
iter_max = 2
index = 1;
Mu = 1;
Beta = 0.5;
xk = X_0;

while index <= iter_max
    figure()

    new_obj = @(X)(obj(X(1), X(2), Mu));
    new_grad = @(X)(grad(X(1), X(2), Mu));
    ta = [10:5e-1:30]; % A
    ts = [5:5e-1:40]; % S
    [tAA,tSS] = meshgrid(ta,ts);
    tDrag = arrayfun(obj, tAA, tSS, ones(size(tAA))*Mu);
    contour(tAA,tSS,tDrag,50);
    qn_log = MajorIterationHistory();
    xk = QuasiNewtonBFGS(linesearch, new_obj, new_grad, xk, e_g, e_a, e_r, qn_log, ls_parameters)
    %x_star = ConjugateGradient(linesearch, new_obj, new_grad, X_0, e_g, e_a, e_r, qn_log, ls_parameters)
    [drag, grad_drag] = plane1.m_DragForce(xk(1), xk(2))

    hold on
    plot(qn_log.x(:,1),qn_log.x(:,2), 'g+-')
    hold on
    index = index + 1;
    Mu = Mu * Beta
    
end
%plot(x_star(1), x_star(2), 'g+')

%============================================================
%           FMINCON WITH SQP                                %
%============================================================
plane2 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);

fmincon_fig = figure;
lb = [1; 1];
ub= [inf; inf];

fmin_log = MajorIterationHistory();
logger_callback = IterationLogger(fmin_log);

options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on', 'OutputFcn', logger_callback,'Display', 'iter');
fmin_obj = @(X)(plane2.m_DragForce(X(1), X(2)));
nonlincon = @(X)(plane2.m_nonLinearConstraint(X(1), X(2), V_min, C_L_max));
[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0,[], [], [], [], lb, ub, nonlincon, options );
plot(fmin_log.x(:,1),fmin_log.x(:,2), 'bd-')
hold on;
plot(x(1), x(2), 'kd');
hold on

