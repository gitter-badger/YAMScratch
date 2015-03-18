function ContourDragTotal(fig, x_star, logObj)
	figure(fig);
	a = [5:1e0:30]; % A
	s = [5:1e0:40]; % S
	[A,S] = meshgrid(a,s);
	Drag = arrayfun(@mdo.DragTotal,A,S);
	contour(A,S,Drag,100);
	%mesh(a,s,Drag);
	xlabel('A');
	ylabel('S');
	hold on;
	plot(logObj.x(:,1), logObj.x(:,2), 'kd-')
	plot(x_star(1), x_star(2), 'k*')
end