clear
clc
close all;
import mdo.*;

%create the filename to save the figure to
d = datestr(clock);
filename = regexprep(d, '\s|-|:', '_');

CONTOUR = true;
if CONTOUR
	a= 10;
	b= 10;
	grid_scale = 1e0;
	x = -a:grid_scale:a;
	y = -b:grid_scale:b;
	%special objective for use with array fun
	obj = @(X,Y)( func6([X,Y].') );
	[X,Y] = meshgrid(x,y);
	tic
	out = arrayfun(obj, X, Y);
	toc

	mesh_fig = figure;
	mesh(x, y, out);

	mesh_filename = strcat('meshplot_', filename);
	saveas(mesh_fig, mesh_filename, 'fig')
	con_fig = figure;
	contour(x,y,out,50);
	con_name = strcat('contour_', filename);
	saveas(con_fig, con_name, 'fig');
	disp('All done')
	exit;
end

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-3;
mu_2 = 0.9;
alpha_init = 1;
alpha_max = 20;
tolerance = 1e-3;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
%create the objective function and the gradient
obj = @func6;
grad = @(X)( mdo.ComplexStepGrad(obj, X, 1e-20));
X_0 = [-.7;
		-.9];

%===============================================
%			Steepest Descent
%===============================================
disp('======Starting Steepest Descent===========')
steep_log = MajorIterationHistory();
%x_star = SteepestDescent(linesearch, obj, grad, X_0, steep_log, ls_parameters);
%===============================================
%			Conjugate gradient method
%===============================================
e_g = 1e-6;
e_a = 1e-6;
e_r = 1e-6;
disp('======Conjugate Gradient===========')
congj_log = MajorIterationHistory();
%x_star = ConjugateGradient(linesearch, obj, grad, X_0, e_g, e_a, e_r, congj_log, ls_parameters);
%===============================================
%			    Quasi Newton Method
%===============================================
disp('======Quasi Newton BFGS===========')
qn_log = MajorIterationHistory();
x_star = QuasiNewtonBFGS(linesearch, obj, grad, X_0, e_g, e_a, e_r, qn_log, ls_parameters);

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

var_filename = strcat('search_', filename);
save(var_filename)
