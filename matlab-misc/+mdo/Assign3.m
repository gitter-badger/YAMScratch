clear
close('all');
import mdo.*

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = 1;
alpha_max = 3;
%create the objective function and the gradient
obj = @(X) (mdo.DragTotal(X(1), X(2)).');
grad = @GradientDragTotal;
X_0 = [25;
		35];

%===============================================
%			Steepest Descent
%===============================================
steep_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, obj, grad, X_0, steep_log, 1e-6);


%plot the objective function
SD_fig = figure();
ContourDragTotal(SD_fig, x_star, steep_log);
figure(SD_fig);

%===============================================
%			Conjugate gradient method
%===============================================
e_g = 1e-6;
e_a = 1e-6;
e_r = 1e-6;
disp('Flat')
congj_log = MajorIterationHistory();
x_star = ConjugateGradient(linesearch, obj, grad, X_0, e_g, e_a, e_r, congj_log)

%plot the objective function
CG_fig = figure();
ContourDragTotal(CG_fig, x_star, congj_log);
figure(CG_fig);
%===============================================
%			    Quasi Newton Method
%===============================================


%plot the objective function
qn_log = MajorIterationHistory();

QN_fig = figure();
%ContourDragTotal(QN_fig, x_star, qn_log)

%===============================================
%			Newton Conjugate Gradient
%===============================================

%plot the objective function
NCG_fig = figure();

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
%now test the three algorithms

x_0 = ones(2,1);
quad2_SD_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, quad_obj_2, quad_grad_2, x_0, quad2_SD_log, 1e-6 );
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

quad2_CG_log = MajorIterationHistory();
x_star_CG = ConjugateGradient(linesearch, quad_obj_2, quad_grad_2, x_0, e_g, e_a, e_r, quad2_CG_log);
hold on
plot(quad2_CG_log.x(:, 1), quad2_CG_log.x(:, 2), 'gd-')
