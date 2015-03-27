clear;
close all;

%we assume an array of N points describing a closed polygon
% points[0] = points[N]
%units in millimeters

%solve for the centers of fillets manually
radius = 5;
f1 =[radius/tand(60/2), radius];
f2 = [(40/tand(60)) - (2*radius*tand(30)), 40];
f3 = [radius/tand(60/2), 80-radius];
f4 = [100 - (radius / tan((pi - atan(8/5))/2)), 5];

temp = [f1; f2; f3; f4];
plot(temp(:,1), temp(:,2), 'kd')
axis('equal')
hold on

%compute the maximum arc step, this is the same for all fillets
miniumum_arc_length = 1;
dtheta = miniumum_arc_length / radius;
%bottom left fillet
start_1 = 5*pi/6;
end_1 = 3*pi/2;
theta_1 = start_1:dtheta:end_1;
if theta_1(end) ~= end_1
	theta_1(end+1) = end_1;
end
%middle left fillet
start_2 = -pi/6;
end_2 = pi/6;
theta_2 = start_2:dtheta:end_2;
if theta_2(end) ~= end_2
	theta_2(end+1) = end_2;
end
%flip theta_2 to be consitant with winding direction
theta_2 = fliplr(theta_2);
%top left fillet
start_3 = pi/2;
end_3 = 7*pi/6;
theta_3 = start_3:dtheta:end_3;
if theta_3(end) ~= end_3
	theta_3(end+1) = end_3;
end
%bottom right fillet
start_4 = 3*pi/2;
end_4 = (3*pi/2) + atan(8/5);
theta_4 = start_4:dtheta:end_4;
if theta_4(end) ~= end_4
	theta_4(end+1) = end_4;
end

points_x(1) = [150];
points_y(1) = [80];

points_x = [points_x (radius*cos(theta_3) + f3(1)) ...
					 (radius*cos(theta_2) + f2(1)) ...
					 (radius*cos(theta_1) + f1(1)) ...
					 (radius*cos(theta_4) + f4(1)) , 150];

points_y = [points_y (radius*sin(theta_3) + f3(2)) ...
					 (radius*sin(theta_2) + f2(2)) ...
					 (radius*sin(theta_1) + f1(2)) ...
					 (radius*sin(theta_4) + f4(2)) , 80];

plot(points_x, points_y, 'bd-')


