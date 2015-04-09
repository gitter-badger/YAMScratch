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

fun = @plane7.m_LandingConstraint;

[scatA, scatS] = meshgrid([5:0.5:30],[5:0.5:40]);

otherA = gpuArray(scatA);
otherS = gpuArray(scatS);
tic 
gcons_out = arrayfun(fun, otherA, otherS, V_min, C_L_max)
toc

tic
Constraint_out = arrayfun(fun, scatA, scatS);
toc


scatA(Constraint_out < 0) = NaN; 
scatS(Constraint_out < 0) = NaN;
scatter(scatA(:), scatS(:), 3, 'g',  'filled')
error

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
tolerance = 5e-5;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
e_g = 1e-6;
e_a = 1e-6;
e_r = 1e-6;

[obj, grad] = mdo.LogBarrierFactory(plane1, V_min, C_L_max);
iter_max = 10
index = 1;
Mu = 1;
Beta = 0.5;
xk = X_0;

hessian = eye(2);
over_all_log = MajorIterationHistory();

[drag, grad_drag] = plane1.m_DragForce(xk(1), xk(2));
[constraint, grad_constraint] = plane1.m_LandingConstraint(xk(1), xk(2), V_min, C_L_max)
%approximate lambda_1 as 
lambda_1 = -1 * (Mu / -constraint);
intial_firstorderopt = norm(grad_drag - lambda_1 * -grad_constraint)

while index <= iter_max
    new_obj = @(X)(obj(X(1), X(2), Mu));
    new_grad = @(X)(grad(X(1), X(2), Mu));
    if DEBUG
        figure()
        ta = [10:2e-2:15]; % A
        ts = [10:2e-2:15]; % S
        [tAA,tSS] = meshgrid(ta,ts);
        tDrag = arrayfun(obj, tAA, tSS, ones(size(tAA))*Mu);
        contour(tAA,tSS,tDrag,100);
    end
    qn_log = MajorIterationHistory();
    if index == 1

    else
        [xk, hessian] = QuasiNewtonBFGS(linesearch, new_obj, new_grad, xk, e_g, e_a, e_r, qn_log, ls_parameters, hessian)
    end
    %add all of this iterations counts to the master list
    for part_iter = 1:qn_log.total_iterations
        over_all_log.appendIteration( qn_log.f(part_iter,:), qn_log.x(part_iter,:),qn_log.g(part_iter,:), qn_log.fevals(part_iter,:), qn_log.dfevals(part_iter,:))
    end
    %compute first order optimality condition
    [drag, grad_drag] = plane1.m_DragForce(xk(1), xk(2));
    [constraint, grad_constraint] = plane1.m_LandingConstraint(xk(1), xk(2), V_min, C_L_max)
    %approximate lambda_1 as 
    lambda_1 = -1 * (Mu / -constraint)
    firstorderopt = norm(grad_drag - lambda_1 * grad_constraint)
    if firstorderopt < 1e-6 * intial_firstorderopt
        break
    end
    index = index + 1;
    Mu = Mu * Beta

    
end
%plot(x_star(1), x_star(2), 'g+')

%============================================================
%           FMINCON WITH SQP                                %
%============================================================
plane2 = mdo.ComputeAirPlane(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);

%fmincon_fig = figure;
lb = [1; 1];
ub= [inf; inf];

fmin_log = MajorIterationHistory();
logger_callback = IterationLogger(fmin_log);

options = optimoptions('fmincon', 'Algorithm', 'sqp' ,'GradObj', 'on', 'GradConstr', 'on', 'OutputFcn', logger_callback,'Display', 'iter');
fmin_obj = @(X)(plane2.m_DragForce(X(1), X(2)));
nonlincon = @(X)(plane2.m_nonLinearConstraint(X(1), X(2), V_min, C_L_max));
[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0,[], [], [], [], lb, ub, nonlincon, options );
hold on;
%plot(x(1), x(2), 'kd');
hold on
plot(fmin_log.x(:,1),fmin_log.x(:,2), 'rd-')
hold on
plot(over_all_log.x(:,1), over_all_log.x(:,2), 'bd-.')
hold on
legend('Unmodified Objective Function', 'Infeasible region', 'SQP', 'Log Barrier' )


%============================================================
%                Plotting Code                              %
%============================================================

only_fmincon_fig = figure;
gradients_fig = subplot(1,2,1);
semilogy([1:fmin_log.total_iterations], fmin_log.optimality, 'kd-')

xlabel('Major Iterations');
ya1 = ylabel('$|\nabla \mathcal{L}|$','Rotation',0,'interpreter','latex');
set(ya1,'Units','Normalized','Position',[-0.17 0.5 0]);

minor_fig = subplot(1,2,2);
semilogy(fmin_log.fevals, fmin_log.optimality, 'kd-');

xlabel('Function Evaluations')
ya2 = ylabel('$|\nabla \mathcal{L}|$','Rotation',0, 'interpreter','latex');
set(ya2,'Units','Normalized','Position',[-0.17 0.5 0]);
legend1 = legend('SQP');
set(legend1, 'Position',[0.592125803489439 0.171707822533567 0.164370982552801 0.106246351430239]);

barrier_fig = figure;
bg_fig = subplot(1,2,1);
semi_hand = semilogy(1,1);
PlotMajorIterationConvergance(barrier_fig, over_all_log, 'kd-');
subplot(bg_fig);
delete(semi_hand);

xlabel('Major Iterations');
ya1 = ylabel('$|\nabla \pi |$','Rotation',0,'interpreter','latex');
set(ya1,'Units','Normalized','Position',[-0.17 0.5 0]);

barrier_minor_fig = subplot(1,2,2);
semi_hand = semilogy(1,1);
PlotMinorIterationConvergance(barrier_fig, over_all_log, 'kd-', barrier_minor_fig);
subplot(minor_fig);
delete(semi_hand)
xlabel('Function Evaluations')
ya2 = ylabel('$|\nabla \pi|$','Rotation',0,'interpreter','latex');
set(ya2,'Units','Normalized','Position',[-0.17 0.5 0]);
legend1 = legend('Log Barrier');
set(legend1, 'Position',[0.73 0.8 0.164370982552801 0.106246351430239]);

