function [x_list,y_list,z_list] = make_step(x_list,y_list,z_list,A,B,C)
	x = x_list(end);
	y = y_list(end);
	z = z_list(end);

	x_step = (B^2*C^2*((x-2)^2) + A^2*C^2*(y^2) + A^2*B^2*(z^2) - A^2*B^2*C^2);
	y_step = (B^2*C^2*(x^2)+ A^2*C^2*((y+2)^2) + A^2*B^2*(z^2) - A^2*B^2*C^2);
	both = (B^2*C^2*((x-2)^2) + A^2*C^2*((y+2)^2) + A^2*B^2*(z^2) - A^2*B^2*C^2);


	alll = [abs(x_step),abs(y_step),abs(both)];
	if z > 18
		disp('################')
		disp([x,y,z])
		disp([x_step,y_step,both])
		disp('&&&&&&&&&&&&&&&&')
	end
	if abs(x_step) == min(alll)
		x_list(end+1) = x-2;
		y_list(end+1) = y;
		z_list(end+1) = z;
		disp('made x_step')
		%try again to make a x-step
		%[x_list,y_list,z_list] = make_step(x_list,y_list,z_list,A,B,C);
	elseif abs(y_step) == min(alll)
		y_list(end+1) = y+2;
		x_list(end+1) = x;
		z_list(end+1) = z;
		disp('made y_step')
	else
		%make both steps
		x_list(end+1) = x-2;
		y_list(end+1) = y+2;
		z_list(end+1) = z;
		disp('made both')	
	end
	return
end
