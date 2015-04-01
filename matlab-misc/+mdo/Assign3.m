clear
close('all');
import mdo.*

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-4;
mu_2 = 0.6;
alpha_init = 1;
alpha_max = 20;
tolerance = 1e-6;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
%create the objective function and the gradient
obj = @(X) (mdo.DragTotal(X(1), X(2)).');
grad = @GradientDragTotal;
X_0 = [15;
		35];

%===============================================
%			Steepest Descent
%===============================================
steep_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, obj, grad, X_0, steep_log, ls_parameters);


%plot the objective function
% SD_fig = figure();
% ContourDragTotal(SD_fig, steep_log, 'kd-', x_star);
% figure(SD_fig);

%===============================================
%			Conjugate gradient method
%===============================================
 e_g = 1e-6;
 e_a = 1e-6;
 e_r = 1e-6;
congj_log = MajorIterationHistory();
x_star = ConjugateGradient(linesearch, obj, grad, X_0, e_g, e_a, e_r, congj_log, ls_parameters);

%plot the objective function
% CG_fig = figure();
% ContourDragTotal(CG_fig, congj_log, 'ko-.', x_star);
% figure(CG_fig);
% title('Conjugate Gradient Method')
%===============================================
%			    Quasi Newton Method
%===============================================


%plot the objective function
qn_log = MajorIterationHistory();
x_star = QuasiNewtonBFGS(linesearch, obj, grad, X_0, e_g, e_a, e_r, qn_log, ls_parameters);
% QN_fig = figure();
% ContourDragTotal(QN_fig, qn_log, 'kd-', x_star);
% figure(QN_fig);

%===============================================
%			Newton Conjugate Gradient
%===============================================

%plot the objective function
%NCG_fig = figure();

%===============================================
% 			Plotting all together
%===============================================
all_algs_fig = figure;

figure(all_algs_fig)
ContourDragTotal(all_algs_fig, steep_log, 'kd-', x_star);
ContourDragTotal(all_algs_fig, congj_log, 'ro-.');
ContourDragTotal(all_algs_fig, qn_log, 'b<--');
figure(all_algs_fig);
legend1 = legend('Drag = f(A,S)', 'x_{*}','Steepest descent', 'Conjugate gradient', 'Quasi Newton BFGS');
set(legend1, 'Position',[0.7 0.5 0.15 0.067]);
xlabel('A')
ylabel('S')

both_gradients_fig = figure;
gradients_fig = subplot(1,2,1);
semilogy(1,1)
PlotMajorIterationConvergance(both_gradients_fig, steep_log, 'kd-');
PlotMajorIterationConvergance(both_gradients_fig, congj_log, 'ro-.');
PlotMajorIterationConvergance(both_gradients_fig, qn_log, 'b<--');
subplot(gradients_fig);
xlabel('Major Iterations');
ya1 = ylabel('|g_k|/|g_0|','Rotation',0);
set(ya1,'Units','Normalized','Position',[-0.17 0.5 0]);

minor_fig = subplot(1,2,2);
semi_hand = semilogy(1,1);
PlotMinorIterationConvergance(both_gradients_fig, steep_log, 'kd-', minor_fig);
PlotMinorIterationConvergance(both_gradients_fig, congj_log, 'ro-.', minor_fig);
PlotMinorIterationConvergance(both_gradients_fig, qn_log, 'b<--', minor_fig);
subplot(minor_fig);
delete(semi_hand)
xlabel('Function Evaluations')
ya2 = ylabel('|g_k|/|g_0|','Rotation',0);
set(ya2,'Units','Normalized','Position',[-0.17 0.5 0]);
legend1 = legend('Steepest descent', 'Conjugate gradient', 'Quasi Newton BFGS');
set(legend1, 'Position',[0.592125803489439 0.171707822533567 0.164370982552801 0.106246351430239]);


%===============================================
%===============================================

%IMPORTANT: must pass in a column vector for X
quad_obj_factory = @(n) (@(X) (sum((X.^2)./[1:n].') ));
quad_grad_factory = @(n) (@(X) (2*X./[1:n].'));
%varying dimensionality of objective for algorithm comparison
quad_obj_2 = quad_obj_factory(2);
quad_grad_2 = quad_grad_factory(2);
quad_obj_10 = quad_obj_factory(10);
quad_grad_10 = quad_grad_factory(10);
quad_obj_50 = quad_obj_factory(50);
quad_grad_50 = quad_grad_factory(50);

%===============================================
%         now test the three algorithms
%===============================================
simple_quad = figure();
figure(simple_quad)
x_0 = ones(2,1);
quad2_SD_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, quad_obj_2, quad_grad_2, x_0, quad2_SD_log, ls_parameters );
x1 = [-2:1e-1:2];
x2 = [-2:1e-1:2];
[X1, X2] = meshgrid(x1,x2);
temp_fun = @(x,y) (quad_obj_2([x; y]));
quad_out = arrayfun(temp_fun ,X1, X2);
contour(x1, x2, quad_out, 30);
hold on
plot(quad2_SD_log.x(:, 1), quad2_SD_log.x(:, 2), 'kd-')
hold on
plot(x_star(1), x_star(2), 'k*')
xlabel('X_1','FontSize', 14)
y2_all = ylabel('X_2','Rotation',0,'FontSize', 14);
set(y2_all,'Units','Normalized','Position',[-0.07 0.5 0]);



quad2_CG_log = MajorIterationHistory();
x_star_CG = ConjugateGradient(linesearch, quad_obj_2, quad_grad_2, x_0, e_g, e_a, e_r, quad2_CG_log, ls_parameters);
hold on
plot(quad2_CG_log.x(:, 1), quad2_CG_log.x(:, 2), 'ro-.')

quad2_QN_log = MajorIterationHistory();
x_star_QN = QuasiNewtonBFGS(linesearch, quad_obj_2, quad_grad_2, x_0, e_g, e_a, e_r, quad2_QN_log, ls_parameters);
hold on
plot(quad2_QN_log.x(:, 1), quad2_QN_log.x(:, 2), 'b<--')
legend2 = legend('f(x_1,x_2)' ,'X_*','Steepest descent', 'Conjugate gradient', 'Quasi Newton BFGS');
set(legend2, 'Position',[0.68 0.322 0.2 0.15]);

quad_converge = figure();

quad2_gradients_fig = subplot(3,2,1);
semilogy(1,1)
PlotMajorIterationConvergance(quad_converge, quad2_SD_log, 'kd-', quad2_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad2_CG_log, 'ro-.', quad2_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad2_QN_log, 'b<--', quad2_gradients_fig);
subplot(quad2_gradients_fig);
xlabel('Major Iterations');
yq2 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yq2,'Units','Normalized','Position',[-0.17 0.5 0]);
title('Quadratic objective in \Re^{2}', 'FontSize', 14)



quad2_minor_fig = subplot(3,2,2);
semilogy(1,1)
PlotMinorIterationConvergance(quad_converge, quad2_SD_log, 'kd-',quad2_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad2_CG_log, 'ro-.', quad2_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad2_QN_log, 'b<--',quad2_minor_fig);
subplot(quad2_minor_fig);
xlabel('Function Evaluations')
yqM2 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yqM2,'Units','Normalized','Position',[-0.17 0.5 0]);
title('Quadratic objective in \Re^{2}', 'FontSize', 14)


%===============================================
% 		Computation for Higher Dimensions
%===============================================
x_0 = ones(10,1);

quad10_SD_log = MajorIterationHistory();
x_star_SD_10 = SteepestDescent(linesearch, quad_obj_10, quad_grad_10, x_0, quad10_SD_log, ls_parameters);

quad10_CG_log = MajorIterationHistory();
x_star_CG_10 = ConjugateGradient(linesearch, quad_obj_10, quad_grad_10, x_0, e_g, e_a, e_r, quad10_CG_log, ls_parameters);

quad10_QN_log = MajorIterationHistory();
x_star_QN_10 = QuasiNewtonBFGS(linesearch, quad_obj_10, quad_grad_10, x_0, e_g, e_a, e_r, quad10_QN_log, ls_parameters);


quad10_gradients_fig = subplot(3,2,3);
%we have to make this semilog plot so that the plot type will be correct
%it does not work correctly if we remove this hack
%therefore we must delete this data series when we want to generate a legend
%which we do below
semilogy(1,1);
PlotMajorIterationConvergance(quad_converge, quad10_SD_log, 'kd-', quad10_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad10_CG_log, 'ro-.', quad10_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad10_QN_log, 'b<--', quad10_gradients_fig);
subplot(quad10_gradients_fig);
xlabel('Major Iterations');
yq10 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yq10,'Units','Normalized','Position',[-0.17 0.55 0]);
title('Quadratic objective in \Re^{10}', 'FontSize', 14)

quad10_minor_fig = subplot(3,2,4);
semi_handle = semilogy(1,1);
PlotMinorIterationConvergance(quad_converge, quad10_SD_log, 'kd-', quad10_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad10_CG_log, 'ro-.', quad10_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad10_QN_log, 'b<--', quad10_minor_fig);
subplot(quad10_minor_fig);
delete(semi_handle);
yqM10 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yqM10,'Units','Normalized','Position',[-0.17 0.55 0]);
xlabel('Function Evaluations');
legend1 = legend('Steepest descent', 'Conjugate gradient', 'Quasi Newton BFGS');
set(legend1,...
    'Position',[0.768665231431643 0.45 0.192680301399354 0.0650929899856939]);
title('Quadratic objective in \Re^{10}', 'FontSize', 14)


%Dimension R50
x_0 = ones(50,1);
quad50_SD_log = MajorIterationHistory();
x_star_SD_50 = SteepestDescent(linesearch, quad_obj_50, quad_grad_50, x_0, quad50_SD_log, ls_parameters);

quad50_CG_log = MajorIterationHistory();
x_star_CG_50 = ConjugateGradient(linesearch, quad_obj_50, quad_grad_50, x_0, e_g, e_a, e_r, quad50_CG_log, ls_parameters);

quad50_QN_log = MajorIterationHistory();
x_star_QN_50 = QuasiNewtonBFGS(linesearch, quad_obj_50, quad_grad_50, x_0, e_g, e_a, e_r, quad50_QN_log, ls_parameters);


quad50_gradients_fig = subplot(3,2,5);
semilogy(1,1)
PlotMajorIterationConvergance(quad_converge, quad50_SD_log, 'kd-', quad50_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad50_CG_log, 'ro-.', quad50_gradients_fig);
PlotMajorIterationConvergance(quad_converge, quad50_QN_log, 'b<--', quad50_gradients_fig);
subplot(quad50_gradients_fig);
xlabel('Major Iterations');
yq50 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yq50,'Units','Normalized','Position',[-0.17 0.55 0]);
title('Quadratic objective in \Re^{50}', 'FontSize', 14)

quad50_minor_fig = subplot(3,2,6);
semilogy(1,1)
PlotMinorIterationConvergance(quad_converge, quad50_SD_log, 'kd-', quad50_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad50_CG_log, 'ro-.', quad50_minor_fig);
PlotMinorIterationConvergance(quad_converge, quad50_QN_log, 'b<--', quad50_minor_fig);
subplot(quad50_minor_fig);
xlabel('Function Evaluations')
yqM50 = ylabel('|g_k|/|g_0|','Rotation',0);
set(yqM50,'Units','Normalized','Position',[-0.17 0.55 0]);
title('Quadratic objective in \Re^{50}', 'FontSize', 14)
