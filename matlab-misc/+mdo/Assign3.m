clear
close('all');
import mdo.*

linesearch = @mdo.ProfLinesearch;
%Conjugate gradient method

%Quasi Newton Method

%Newton Conjugate Gradient

%must take a column vector
quad_obj_factory = @(n) (@(X) (sum((X.^2)./[1:n].') ));
%varying dimensionality of objective for algorithm comparison
quad_obj_2 = quad_obj_factory(2);
quad_obj_10 = quad_obj_factory(10);
quad_obj_50 = quad_obj_factory(50);
%now test the three algorithms
