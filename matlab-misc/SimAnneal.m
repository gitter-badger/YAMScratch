clear
clc
close all;


%first repeat the simulated annealing accpetance trials m times.
%in each time, generate an exploratory point z_n close to the current
%iterate x_n and a SA trial along the direction sign(f(x_n) - f(z_n))*(z_n-x_n)
%if the trial is accepted, proceed to next iteration
%otherwise take the speed descnet direction as the search direction

%create intial array of test points on interval [L,U]

initial_x = 1;
mesh_size = 0.1; %should be greater than zero
gam = 3; % SA trial point radius
tau = 1; % must be greater than 1
omega_neg = -1; % must be less than or equal to -1
omega_pos = 1; % must be greater or equal to 1
epsilon = 0.01;
T_max = 700;
T_min = 0;
T_init = T_max;
epoch_length = 35;
cool_ratio = 0.98; %should be (0.5,0.99)

m = 5; %search times
m_accumulator = 3;%how many times we must accept SA trial before moving

for search = 1:m

end