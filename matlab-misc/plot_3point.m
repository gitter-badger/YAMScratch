function [] = plot_3point(x,y,z,fig)
    %select the figure
    figure(fig)
    axis equal
    hold on
    %XY plane
	one_x = [x-1,x-1,x+1,x+1];
	one_y = [y+1,y-1,y-1,y+1];
	one_z = [z+1,z+1,z+1,z+1];
	fill3(one_x,one_y,one_z,'c')
	fill3(one_x,one_y,-one_z,'c')
	%XZ plane
	two_x = [x-1,x-1,x+1,x+1];
	two_y = [y+1,y+1,y+1,y+1];
	two_z = [z+1,z-1,z-1,z+1];
	fill3(two_x,two_y,two_z,'m')
	fill3(two_x,two_y,-two_z,'m')
	%YZ plane
	three_x = [x+1,x+1,x+1,x+1];
	three_y = [y-1,y-1,y+1,y+1];
	three_z = [z+1,z-1,z-1,z+1];
	fill3(three_x,three_y,three_z,'y')
	fill3(three_x,three_y,-three_z,'y')
end