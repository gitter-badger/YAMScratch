clear('all');
clc
close all
import mdo.*

GRAPH = false;
DEBUG = true;

wing = AeroElasticWing();

tiptwist = -1;

thick = wing.thickRoot * (1 - ((1-wing.taper)/(wing.span/2))*wing.yElem);   % (inches)
jigtwist = wing.yPanel *(tiptwist/(wing.span/2)); % (degrees)

[L, D, W, alpha, lift, twist, uz, maxStress] = wing.MDA(thick, jigtwist);

L
D
W
alpha
maxStress

tmp = exp(wing.range*6076.11549*(wing.sfc*D)/(wing.V*L));
Wfuel = W*(tmp - 1.0)/tmp % (lb)

otherW = wing.fuelWeight(W, D, L)

%============================================================
%                			MDF                             %
%============================================================

fmin_log = MajorIterationHistory();
logger_callback = IterationLogger(fmin_log);

options = optimoptions('fmincon', 'Algorithm', 'interior-point' , ...
	     'GradObj', 'off', 'GradConstr', 'off', ...
	     'OutputFcn', logger_callback,'Display', 'iter');

%constrain the root twist to be zero
jigtwist(1) = 0;
X_0 = vertcat(thick, jigtwist);

lb = vertcat(1e-10 * ones(wing.nElem, 1), -inf* ones(wing.nPanel, 1));
ub = [];
A = [];
b = [];
Aeq = [];
beq = [];
nonlincon = @(X) (wing.OneNonlinearConstraintToRuleThemAll(X));
fmin_obj = @(X) (wing.OneObjectiveToRuleThemAll(X));

[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0, A, b, Aeq, beq, lb, ub, nonlincon, options );

%============================================================
%                Plotting Code                              %
%============================================================
if GRAPH
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
	legend1 = legend('Interior Point');
	set(legend1, 'Position',[0.592125803489439 0.171707822533567 0.164370982552801 0.106246351430239]);
end
%============================================================
%                			IDF                             %
%============================================================

fmin_log_IDF = MajorIterationHistory();
logger_callback = IterationLogger(fmin_log_IDF);

options = optimoptions('fmincon', 'Algorithm', 'SQP' , ...
	     'GradObj', 'off', 'GradConstr', 'off', ...
	     'OutputFcn', logger_callback,'Display', 'iter');

%constrain the root twist to be zero
jigtwist(1) = 0;
u_init = zeros(wing.nDOF, 1);
gamma_init = zeros(wing.nPanel, 1);
alpha_init = [0];

X_0 = vertcat(thick, jigtwist, u_init, gamma_init, alpha_init);

lb = vertcat(1e-10 * ones(wing.nElem, 1), -inf* ones(wing.Offsets(5) - (wing.Offsets(2) - 1), 1));
ub = [];
A = [];
b = [];
Aeq = [];
beq = [];
nonlincon = @(X) (wing.OneNonlinearConstraintToRuleThemAll(X));
fmin_obj = @(X) (wing.OneObjectiveToRuleThemAll(X));

error('Description');
[x,fval, exitflag, output, lambda] = fmincon(fmin_obj,X_0, A, b, Aeq, beq, lb, ub, nonlincon, options );

%============================================================
%                Plotting Code                              %
%============================================================
if GRAPH
	only_fmincon_fig = figure;
	gradients_fig = subplot(1,2,1);
	semilogy([1:fmin_log_IDF.total_iterations], fmin_log_IDF.optimality, 'kd-')

	xlabel('Major Iterations');
	ya1 = ylabel('$|\nabla \mathcal{L}|$','Rotation',0,'interpreter','latex');
	set(ya1,'Units','Normalized','Position',[-0.17 0.5 0]);

	minor_fig = subplot(1,2,2);
	semilogy(fmin_log_IDF.fevals, fmin_log_IDF.optimality, 'kd-');

	xlabel('Function Evaluations')
	ya2 = ylabel('$|\nabla \mathcal{L}|$','Rotation',0, 'interpreter','latex');
	set(ya2,'Units','Normalized','Position',[-0.17 0.5 0]);
	legend1 = legend('Interior Point');
	set(legend1, 'Position',[0.592125803489439 0.171707822533567 0.164370982552801 0.106246351430239]);
end

