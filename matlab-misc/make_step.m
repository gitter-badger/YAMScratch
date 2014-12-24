function [x_list,y_list] = make_step(x_list,y_list,A,B)
	x = x_list(end);
	y = y_list(end);
	x_step = abs((B^2)*((x-2)^2) + (A^2)*(y^2) - A^2*B^2);
	y_step = abs((B^2)*(x^2) + (A^2)*((y+2)^2) - A^2*B^2);
	both = abs((B^2)*((x-2)^2) + (A^2)*((y+2)^2) - A^2*B^2);
	alll = [x_step,y_step,both];
	if x_step == min(alll)
		x = x - 2;
		%try again to make a
		x_list(end+1) = x;
		y_list(end+1) = y;
		[x_list,y_list] = make_step(x_list,y_list,A,B);
	elseif y_step == min(alll)
		y = y+2;
		y_list(end+1) = y;
		x_list(end+1) = x;
	else
		x_list(end+1) = x-2;
		y_list(end+1) = y+2;	
	end
	return
end
