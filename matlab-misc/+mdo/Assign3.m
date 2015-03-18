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
X_0 = [10;
		20];

%===============================================
%			Steepest Descent
%===============================================
steep_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, obj, grad, X_0, steep_log, 1e-6);


%plot the objective function
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[A,S] = meshgrid(a,s);
Drag = arrayfun(@mdo.DragTotal,A,S);
SD_fig = figure();
contour(A,S,Drag,100);
%mesh(a,s,Drag);
xlabel('A');
ylabel('S');
hold on;
plot(steep_log.x(:,1), steep_log.x(:,2), 'kd-')
plot(x_star(1), x_star(2), 'k*')

%===============================================
%			Conjugate gradient method
%===============================================

%plot the objective function
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[A,S] = meshgrid(a,s);
Drag = arrayfun(@mdo.DragTotal,A,S);
CG_fig = figure();
contour(A,S,Drag,100);
%mesh(a,s,Drag);
xlabel('A');
ylabel('S');
%===============================================
%			    Quasi Newton Method
%===============================================


%plot the objective function
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[A,S] = meshgrid(a,s);
Drag = arrayfun(@mdo.DragTotal,A,S);
QN_fig = figure();
contour(A,S,Drag,100);
%mesh(a,s,Drag);
xlabel('A');
ylabel('S');
%===============================================
%			Newton Conjugate Gradient
%===============================================

%plot the objective function
a = [5:1e0:30]; % A
s = [5:1e0:40]; % S
[A,S] = meshgrid(a,s);
Drag = arrayfun(@mdo.DragTotal,A,S);
NCG_fig = figure();
contour(A,S,Drag,100);
%mesh(a,s,Drag);
xlabel('A');
ylabel('S');

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
quad2_log = MajorIterationHistory();
x_star = SteepestDescent(linesearch, quad_obj_2, quad_grad_2, x_0, quad2_log, 1e-6 );
x1 = [-2:1e-1:2];
x2 = [-2:1e-1:2];
[X1, X2] = meshgrid(x1,x2);
temp_fun = @(x,y) (quad_obj_2([x; y]));
quad_out = arrayfun(temp_fun ,X1, X2);
contour(x1, x2, quad_out, 30);
hold on
plot(quad2_log.x(:, 1), quad2_log.x(:, 2), 'kd-')
hold on
plot(x_star(1), x_star(2), 'k*')
