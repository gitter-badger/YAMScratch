function [grad] = GradientDragTotal(x)
	cs_h = 1e-60;
	%make sure we return a column vector
	grad(1,1) = imag(mdo.DragTotal(x(1)+ 1i * cs_h, x(2))) / cs_h;
	grad(2,1) = imag(mdo.DragTotal(x(1), x(2) + 1i * cs_h)) / cs_h;
	return
end