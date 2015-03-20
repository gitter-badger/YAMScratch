function ContourDragTotal(fig, logObj, format_string, varargin)
	figure(fig);
	hold on;
	if nargin == 4
		a = [5:1e0:30]; % A
		s = [5:1e0:40]; % S
		[A,S] = meshgrid(a,s);
		Drag = arrayfun(@mdo.DragTotal,A,S);
		contour(A,S,Drag,100);
		%mesh(a,s,Drag);
		xlabel('A');
		ylab = ylabel('S','Rotation',0);
		set(ylab,'Units','Normalized','Position',[-0.7 0.5 0]);
		x_star = varargin{1};
		plot(x_star(1), x_star(2), 'k*');
	end
	%if the optimum point is not given then assume it is already plotted
	plot(logObj.x(:,1), logObj.x(:,2), format_string)
end
