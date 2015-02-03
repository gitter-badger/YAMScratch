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
U = 1000;
L = -U;
x_k = rand('double')*(U-L) - U;
fprintf(fid, 'Starting x: %f\n', X );
fprintf(fid, 'x, f(x), g(x)\n' ); %tell the format

N = 1; %
epoch_length = 2*N; %twice

mesh_size = (U-L) /5; %initialize the mesh size for a jump of 1/5 interval

epsilon = 0.001; %neighborhood radius
trial_radius = epsilon; % SA trial point radius


x_nearby = x_k + (rand*2 -1) *trial_radius;

[f_x_k, g_x_k] = func1(x_k);
fprintf(fid, 'Starting: %10.10f,%10.10f,%10.10f\n',[x_k, f_x_k, g_x_k] );
[f_near, g_near] = func1(x_nearby);
fprintf(fid, 'Nearby: %10.10f,%10.10f,%10.10f\n',[x_nearby, f_near, g_near] );

T_max = -abs(f_near - f_x_k) / log(0.9);
T_min = min([1,T_max]*0.001);
T_init = T_max;
fprintf(fid, 'T_max: %10.10f \n T_min: %10.10f\n', [T_max,T_min]);

memo = false; % flag to skip recalculating first time through loop

cool_ratio = 0.98; %should be (0.5,0.99)


for search = 1:epoch_length
	%log which iteration we are on
	fprintf(fid, 'Iteration: %i \n', [search] );
	%evaluate the test point x_k
	if(memo)
		[f_x_k, g_x_k] = func1(x_k);
		fprintf(fid, '%10.10f,%10.10f,%10.10f\n',[x_k, f_x_k, g_x_k] );
		memo = true; 
	end
	
	%generate an exploring point z_k
	z_k = rand * epsilon + x_k;
	%evaulate f(z_k)
	[f_z_k, g_z_k] = func1(z_k);
	fprintf(fid, '%10.10f,%10.10f,%10.10f\n',[z_k, f_z_k, g_z_k] );
	nu = 0.1 + (1-0.1) * rand;
	if (f_z_k <= f_x_k)
		%the expression is simplified by the fact that x is a one dimensional vector
		x_SA = x_k + nu*trial_radius * ((z_k - x_k)/abs(z_k-x_k));
	else 
		x_SA = x_k - nu*trial_radius * ((z_k - x_k)/abs(z_k-x_k));
	end
	%evaluate f(x_SA) and accept it conditionally
	[f_x_SA, g_x_SA] = func1(x_SA);
	fprintf(fid, '%10.10f,%10.10f,%10.10f\n\n',[x_SA, f_x_SA, g_x_SA] );
	if(f_x_SA - f_x_k < 0)

	elseif ()
		
	else % do local PS
		if (f_prime_x == 0)
			%return, we have found a minimum
			break;
		%test using the derivative
		else
			if

		end
	end
end

fclose(fid);
