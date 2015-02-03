clear
clc
close all;
%Implementation of algorithm described in:
%	Simulated Annealing Based on Pattern Search
%	with Derivative Information
%		Fengming Ma
%	International Mathematical Forum, 3, 2008, no. 37, 1835 - 1840

%%ALGORITHM
%first repeat the simulated annealing accpetance trials m times.
%in each time, generate an exploratory point z_n close to the current
%iterate x_n and a SA trial along the direction sign(f(x_n) - f(z_n))*(z_n-x_n)
%if the trial is accepted, proceed to next iteration
%otherwise take the speed descnet direction as the search direction

%open a loggin file to store each evaluation of expensive function
fid = fopen('results.txt','at');

%randomly pick a intial starting point on interval [-1000,1000]
X = rand('double')*2000 - 1000;
fprintf(fid, 'Starting x: %f\n', X );




N = 100; %size of the inital value array 
epoch_length = 2*N; %twice



mesh_size = 0.1; %should be greater than zero
trial_radius = 3; % SA trial point radius
tau = 1.1; % must be greater than 1
omega_neg = -1; % must be less than or equal to -1
omega_pos = 1; % must be greater or equal to 1
epsilon = 0.001; %neighborhood radius

T_max = func1(x_0);
T_min = min([1,T_max]*0.001);
T_init = T_max;

cool_ratio = 0.98; %should be (0.5,0.99)


for search = 1:m


	fprintf(fid,'%f %f',[f_x, f_prime_x]); %writing the value
end

fclose(fid);