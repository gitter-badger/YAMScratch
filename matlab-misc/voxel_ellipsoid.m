clear
clc
close all

A = 101; %x axis width
B = 77; %y axis width
C = 50; %z axis width

x_offset = mod(A+1,2); %set the coordinate we start from
y_offset = mod(B+1,2);
z_offset = mod(C+1,2);

% the points are doubled to remove fractional sizes, we will map back later
% start in first octant
fig = figure
X = [0:0.001:A];
Y = sqrt((1-X.^2/A^2)*B^2);
plot(X,Y)
hold on
plot([-X],Y,'r')
axis equal
hold on
%identify target points
inside = [];
outside = [];

for x = [x_offset:2:A-1]
	for y = [y_offset:2:B-1]
		determ = (B^2)*(x^2) + (A^2)*(y^2) - A^2*B^2;
		if determ > 0
			plot(x,y,'rd')
			hold on
			outside(end+1,:) = [x,y];
		else
			corn_x = [x-1,x-1,x+1,x+1,x-1];
			corn_y = [y+1,y-1,y-1,y+1,y+1];
			if x ~= 0 && y~= 0
				%fill(corn_x,corn_y,'g')
				hold on
				fill(-corn_x,corn_y,'b')
				hold on
				fill(-corn_x,-corn_y,'m')
				hold on
				fill(corn_x,-corn_y,'y')
				hold on
			else
				fill(corn_x,corn_y,'c')
				hold on
				fill(-corn_x,-corn_y,'c')
				hold on
			end
			plot(x,y,'bd')
			hold on
			inside(end+1,:) = [x,y];
		end
	end
end
%% Start the reveal
% XY
xy_x_list = [A-1];
xy_y_list = [y_offset];
while xy_y_list(end) <= B-2
	[xy_x_list,xy_y_list] = make_step(xy_x_list,xy_y_list,A,B);
	%there may still be some x error
end
%burn down any remaining x coordinate
while xy_x_list(end) > 1
		xy_x_list(end+1) = xy_x_list(end)- 2;
		xy_y_list(end+1) = xy_y_list(end);
end
%END XY
% START XZ
xz_x_list = [A-1];
xz_z_list = [z_offset];
while xz_z_list(end) <= C-2
	[xz_x_list,xz_z_list] = make_step(xz_x_list,xz_z_list,A,C);
	%there may still be some x error
end
%burn down any remaining x coordinate
while xz_x_list(end) > 1
		xz_x_list(end+1) = xz_x_list(end)- 2;
		xz_z_list(end+1) = xz_z_list(end);
end
%END XZ
%START YZ
yz_y_list = [B-1];
yz_z_list = [z_offset];
while yz_z_list(end) <= C-2
	[yz_y_list,yz_z_list] = make_step(yz_y_list,yz_z_list,B,C);
	%there may still be some x error
end
%burn down any remaining x coordinate
while yz_y_list(end) > 1
		yz_y_list(end+1) = yz_y_list(end)- 2;
		yz_z_list(end+1) = yz_z_list(end);
end
%END YZ

%% START filling in
calc_A = A;
calc_B = B;
x_list = [calc_A-1];
y_list = [y_offset];
while y_list(end) <= calc_B-2
	[x_list,y_list] = make_step(x_list,y_list,calc_A,calc_B);
	%there may still be some x error
end
%burn down any remaining x coordinate
while x_list(end) > 1
		x_list(end+1) = x_list(end)- 2;
		y_list(end+1) = y_list(end);
end


figure
for count = 1:length(xy_x_list)
	x = xy_x_list(count);
	y = xy_y_list(count);
	z = z_offset;
	plot3(x,y,z,'bd')
	plot3(-x,y,z,'bd')
	plot3(-x,-y,z,'bd')
	plot3(x,-y,z,'bd')

	hold on
	
end

for count = 1:length(xz_x_list)
	x = xz_x_list(count);
	z = xz_z_list(count);
	y = y_offset;
	plot3(x,y,z,'rd')
	plot3(-x,y,z,'rd')
	plot3(-x,y,-z,'rd')
	plot3(x,y,-z,'rd')
	hold on
	
end

for count = 1:length(yz_y_list)
	y = yz_y_list(count);
	z = xz_z_list(count);
	x = x_offset;
	plot3(x,y,z,'gd')
	plot3(x,-y,z,'gd')
	plot3(x,-y,-z,'gd')
	plot3(x,y,-z,'gd')
	hold on
	
end
axis equal