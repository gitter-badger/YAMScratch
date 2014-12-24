clear
clc
close all

A = 101; %x axis width
B = 77; %y axis width
C = 10; %z axis width

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

%axis([-A-1,A+1,-1,B+1])
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

x = A-1;
y = y_offset;
while y <= B-2
	[x,y] = make_step(x,y,A,B,fig);
	%there may still be some x error
end
%plot the most recent value
corn_x = [x-1,x-1,x+1,x+1,x-1];
corn_y = [y+1,y-1,y-1,y+1,y+1];
fill(corn_x,corn_y,'k')
hold on
%burn down any remaining x coordinate
while x > 1
		x = x - 2;
		corn_x = [x-1,x-1,x+1,x+1,x-1];
		corn_y = [y+1,y-1,y-1,y+1,y+1];
		fill(corn_x,corn_y,'k')
		hold on
end
