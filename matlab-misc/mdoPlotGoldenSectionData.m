function [] = mdoPlotGoldenSectionData(cost_data, fig, tolerance, z_space)
% Purpose: Create the nice convergance ticks on plot for Golden Section Method
% Inputs:
% cost_data - the cost_data matrix produced by the mdoGoldenSection
%		cost_data = [a1, f(a1), b1, f(b1);
%					 a2, f(a2), b2, f(b2);
%					 ...
%					 aj, f(aj), bj, f(bj)]
% fig - the figure handle to plot on
% tolerance - smalled interval size to show on plot
% z_space - the z-spacing of the ticks, choose something based on the y-scale of your plot
	figure(fig)
	[r,c] = size(cost_data);
	for index = 1:r
		if (cost_data(index,3) - cost_data(index,1) < tolerance)
			stop_index = index;
			break
		end
	end
	%take the approximation of the function value at one side of the interval
	%as the z value starting the z buffer
	start_z = cost_data(stop_index,2);
	%the spacing on the z-axis for each element
	z_spacing = 0.5;
	tick_size = 0.3*z_spacing;
	%work backwards from the smalles interval we consider
	index = 1;
	for level = stop_index:-1:1
		hold on
		left_ticks = [cost_data(level,1),cost_data(level,1)];
		right_ticks = [cost_data(level,3),cost_data(level,3)];
		y_ticks = [-tick_size, tick_size];
		y_ticks = y_ticks + start_z + z_spacing*index;
		plot(left_ticks,y_ticks,'k')
		hold on
		plot(right_ticks,y_ticks,'k');
		hold on
		plot([cost_data(level,1),cost_data(level,3)],[1,1]*(start_z + z_spacing*index),'k')
		index = index + 1;
	end	
	return
end