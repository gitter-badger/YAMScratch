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
%plot(temp(:,1), temp(:,2), 'kd')
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
                     (radius*cos(theta_4) + f4(1)), 150];

points_y = [points_y (radius*sin(theta_3) + f3(2)) ...
                     (radius*sin(theta_2) + f2(2)) ...
                     (radius*sin(theta_1) + f1(2)) ...
                     (radius*sin(theta_4) + f4(2)), 80];

plot(points_x, points_y, 'k-')
hold on

%compute the offset for the last corner
deposition_width = 2.5;
path_offset = deposition_width/2; 

ur_x = 150 - path_offset/tan(atan(8/5)/2);
ur_y = 80 - path_offset;

edge_path_x = [ur_x ((radius- path_offset)*cos(theta_3) + f3(1)) ...
                     ((radius+ path_offset)*cos(theta_2) + f2(1)) ...
                     ((radius - path_offset)*cos(theta_1) + f1(1)) ...
                     ((radius - path_offset)*cos(theta_4) + f4(1)), ur_x];

edge_path_y = [ur_y ((radius- path_offset)*sin(theta_3) + f3(2)) ...
                     ((radius+ path_offset)*sin(theta_2) + f2(2)) ...
                     ((radius- path_offset)*sin(theta_1) + f1(2)) ...
                     ((radius- path_offset)*sin(theta_4) + f4(2)), ur_y];

plot(edge_path_x, edge_path_y, 'r--')
hold on
overlap = 0.95;
bound_offset = deposition_width * overlap + path_offset;
br_x = 150 - bound_offset/tan(atan(8/5)/2);
br_y = 80 - bound_offset;

inner_bound_x = [br_x ((radius- bound_offset)*cos(theta_3) + f3(1)) ...
                     ((radius+ bound_offset)*cos(theta_2) + f2(1)) ...
                     ((radius - bound_offset)*cos(theta_1) + f1(1)) ...
                     ((radius - bound_offset)*cos(theta_4) + f4(1)), br_x];

inner_bound_y = [br_y ((radius- bound_offset)*sin(theta_3) + f3(2)) ...
                     ((radius+ bound_offset)*sin(theta_2) + f2(2)) ...
                     ((radius- bound_offset)*sin(theta_1) + f1(2)) ...
                     ((radius- bound_offset)*sin(theta_4) + f4(2)), br_y];
%plot(inner_bound_x, inner_bound_y, 'r')

%now construct rastering parallel to x axis by brute force
start_y = min(inner_bound_y);
%the first fill raster is offset differently from the others to account for geometry
start_y = start_y;
y_value = start_y;
end_y = max(inner_bound_y);

%store the intersection points from each half of polygon
right_side = [];
left_side = [];

while y_value < end_y
    %test the segments brute force wise
    intersections = [];
    for index = 1:length(inner_bound_x)-1
        if (inner_bound_y(index) < y_value) &(inner_bound_y(index+1) > y_value)
            t = (y_value - inner_bound_y(index)) / (inner_bound_y(index+1) - inner_bound_y(index));
            x_point = inner_bound_x(index) + t * (inner_bound_x(index+1) - inner_bound_x(index));
            right_side(end+1, 1) = x_point;
            right_side(end, 2) = y_value;
        %check the other direction
        elseif (inner_bound_y(index) > y_value) & (inner_bound_y(index+1) < y_value)
            t = (y_value - inner_bound_y(index)) / (inner_bound_y(index+1) - inner_bound_y(index));
            x_point = inner_bound_x(index) + t * (inner_bound_x(index+1) - inner_bound_x(index));
            left_side(end+1, 1) = x_point;
            left_side(end, 2) = y_value;

        end
                
    end
    %compute the intersection locations and draw
    y_value = y_value + deposition_width*overlap;

end

%there really should be two endpoints for each slice
assert(isequal(size(right_side), size(left_side)));

full_path = [];
for index = 1:length(left_side(:,1))
    %alternate
    if mod(index, 2)
        full_path(end+1,:) = left_side(index,:);
        full_path(end+1,:) = right_side(index,:);
    else
        full_path(end+1,:) = right_side(index,:);   
        full_path(end+1,:) = left_side(index,:);  
    end
end

plot(full_path(:,1), full_path(:,2), 'b-')
