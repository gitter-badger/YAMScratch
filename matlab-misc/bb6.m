clear
clc
close all;
import mdo.*;

obj = @(X,Y)( func6([X,Y].') );
CONTOUR = false
if CONTOUR
	a= 2;
	b= 2;
	grid_scale = 1e-1;
	x = -a:grid_scale:a;
	y = -b:grid_scale:b;

	[X,Y] = meshgrid(x,y);
	tic
	out = arrayfun(obj, X, Y);
	toc
	%create the filename to save the figure to
	d = datestr(clock);
	prefix = 'meshplot_';
	filename = strcat(prefix, d);
	filename = regexprep(filename, '\s|-|:', '_')

	mesh_fig = figure;
	mesh(x, y, out);
	saveas(mesh_fig, filename, 'fig')
	disp('All done')
end

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-4;
mu_2 = 0.6;
alpha_init = 1;
alpha_max = 20;
tolerance = 1e-6;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
%create the objective function and the gradient
obj = @func6
grad = @gradfunc6;
X_0 = [1;
		1];

%===============================================
%			Steepest Descent
%===============================================
steep_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, obj, grad, X_0, steep_log, ls_parameters);
%===============================================
%			Conjugate gradient method
%===============================================
 e_g = 1e-6;
 e_a = 1e-6;
 e_r = 1e-6;
congj_log = MajorIterationHistory();
x_star = ConjugateGradient(linesearch, obj, grad, X_0, e_g, e_a, e_r, congj_log, ls_parameters);
%===============================================
%			    Quasi Newton Method
%===============================================
qn_log = MajorIterationHistory();
x_star = QuasiNewtonBFGS(linesearch, obj, grad, X_0, e_g, e_a, e_r, qn_log, ls_parameters);

