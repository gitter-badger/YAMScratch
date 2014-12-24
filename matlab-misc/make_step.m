function [new_x,new_y] = make_step(x,y,A,B,fig)
	figure(fig)
	corn_x = [x-1,x-1,x+1,x+1,x-1];
	corn_y = [y+1,y-1,y-1,y+1,y+1];
	fill(corn_x,corn_y,'k')
	hold on
	x_step = abs((B^2)*((x-2)^2) + (A^2)*(y^2) - A^2*B^2);
	y_step = abs((B^2)*(x^2) + (A^2)*((y+2)^2) - A^2*B^2);
	both = abs((B^2)*((x-2)^2) + (A^2)*((y+2)^2) - A^2*B^2);
	alll = [x_step,y_step,both];
	if x_step == min(alll)
		x = x - 2;
		%try again to make a 
		corn_x = [x-1,x-1,x+1,x+1,x-1];
		corn_y = [y+1,y-1,y-1,y+1,y+1];
		fill(corn_x,corn_y,'k')
		hold on
		[x,y] = make_step(x,y,A,B,fig);
	elseif y_step == min(alll)
		y = y+2;
	else
		x = x-2;
		y = y+2;
	end
	new_y = y;
	new_x = x;
	return
end
