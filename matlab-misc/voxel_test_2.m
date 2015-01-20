clear
clc
close all;

A = 75;
B = 100;
C = 35;

x_offset = mod(A+1,2); %set the coordinate we start from
y_offset = mod(B+1,2);
z_offset = mod(C+1,2);

%initialise the error term


%create a figure to plot in
fig = figure;
%the output lists
x_list = [];
y_list = [];
z_list = [];
%we try to drive the error close to zero from the negative side
%start with an x of A-1, a y of y_offset, and a z of z_offset
x = A-1;
y = y_offset;
z = z_offset;
error = (B^2*x^2 + A^2*y^2 - A^2*B^2)*C^2 + A^2*B^2*z^2;
disp('Starting Error');
disp(error);
plot3([A,-A],[B, -B],[C,-C],'kd')
hold on;
plot3(0,0,0,'kd')
hold on

%iterate slice by slice upwards
for k = z_offset:2:C
    z = k;
    y = y_offset;
    disp('Finding Error')
    error = (B^2*x^2 + A^2*y^2 - A^2*B^2)*C^2 + A^2*B^2*z^2;
    disp(error)
    %if the error is greater than zero than decrease x until it is less
    %than zero
    if error < 0
        x_list(end+1) = x;
        y_list(end+1) = y;
        z_list(end+1) = z;
        plot_3point(x,y,z,fig);
    end
    while error > 0
        x_list(end+1) = x;
        y_list(end+1) = y;
        z_list(end+1) = z-2;
        plot_3point(x,y,z-2,fig);
        x = x-2;
        error = (B^2*x^2 + A^2*y^2 - A^2*B^2)*C^2 + A^2*B^2*z^2;
        disp(error);
    end
    %add the start points to a graph
    disp([x,y,z]);
     x_list(end+1) = x;
     y_list(end+1) = y;
     z_list(end+1) = z;
     plot_3point(x,y,z,fig);
    %now progress along the circumfrance
    while y < B-2
        x_step = error + C^2*4*(1-x);
        xy_step = error + C^2*4*(y-x+2);
        y_step = error + C^2*4*(y+1);
        choices = [x_step,xy_step,y_step];
        disp(choices)
        sorted_choices = sort(choices);
        disp(sorted_choices);
        if sorted_choices(3) <= 0
            key = sorted_choices(3);
        elseif sorted_choices(2) <= 0
            key = sorted_choices(2);
        else
            key = sorted_choices(1);
        end
        disp(key)
        %determine which one will allow us to be closest to negative zero
        if key == x_step
            x = x-2;
            disp('making x step');
        elseif key == xy_step
            x = x-2;
            y = y+2;
            disp('making xy step');

        else
            %it is a y step
            y = y+2;
            disp('making y step');

        end
        error = key;
        x_list(end+1) = x;
        y_list(end+1) = y;
        z_list(end+1) = z;
        plot_3point(x,y,z,fig);
    end
    
    
end
