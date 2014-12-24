clear
clc
close all

A = 31; %x axis width
B = 51; %y axis width
C = 31; %z axis width

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
%%Calculate the A and B for each z value
level = [];
z_values = [];
for z = [z_offset:2:C-1]
	index = find(xz_z_list==z);
	level(end+1,:) = [xz_x_list(index(1))+1,yz_y_list(index(1))+1];
	z_values(end+1) = z;
end


%% START filling in
x_list = [];
y_list = [];
z_list = [];
for index = 1:length(z_values)
	calc_A = level(index,1);
	calc_B = level(index,2);

	x_list(end+1) = [calc_A-1];
	y_list(end+1) = [y_offset];
	while y_list(end) <= calc_B-2
		[x_list,y_list] = make_step(x_list,y_list,calc_A,calc_B);
		%there may still be some x error
	end
	%burn down any remaining x coordinate
	while x_list(end) > 1
			x_list(end+1) = x_list(end)- 2;
			y_list(end+1) = y_list(end);
	end
	while length(z_list) < length(x_list)
		z_list(end+1) = z_values(index);
	end
end

% for count = 1:length(x_list)
% 	x = x_list(count);
% 	y = y_list(count);
% 	corn_x = [x-1,x-1,x+1,x+1,x-1];
% 	corn_y = [y+1,y-1,y-1,y+1,y+1];
% 	fill(corn_x,corn_y,'k')
% 	hold on
% end


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
	z = yz_z_list(count);
	x = x_offset;
	plot3(x,y,z,'gd')
	plot3(x,-y,z,'gd')
	plot3(x,-y,-z,'gd')
	plot3(x,y,-z,'gd')
	hold on
	
end

for count = 1:length(x_list)
	x = x_list(count);
	y = y_list(count);
	z = z_list(count);
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

%plot3(x_list,y_list,z_list,'kd')
hold on

axis equal