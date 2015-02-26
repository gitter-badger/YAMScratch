% test the call to wing.m

clear all;
format compact;
format long e;

% set the inputs
D = [0.16; 0.06; 0.03; 0.02; 0.02; 0.02];   % tube diameters

cs = struct; %namespace for complex step
fd = struct; %namespace for finite difference
% call wing
[first_tiptwist, first_u] = wing(D);


cs.dtip = zeros(size(D,1),1);

cs.h = 1e-20;
for index = 1:size(D,1)
    cs.Dc = D; %this is a copy of the diamenter
    cs.Dc(index) = cs.Dc(index) + 1i*cs.h;
    [tipc, uc] = wing(cs.Dc);
    %compute the derivatice by complex step method
    cs.dtip(index) = imag(tipc)/cs.h;
end

%compute the senstivities with respect to diameter one
cs.Dc = D;
cs.Dc(1) = cs.Dc(1) +  1i*cs.h;
[tipc_u,uc_u] = wing(cs.Dc);
cs.du = imag(uc_u) ./ cs.h;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%				Finite difference method			 %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%test different step sizes
n_steps = 300;
fd.h = logspace(-1,-35,n_steps);
fd.dtip = [];
%the sensitivities for varying step size with finite difference
for iteration = 1:n_steps
	for index = 1:size(D,1)
		Df = D;
		Df(index) = Df(index) + fd.h(iteration);
		[tiptwist, u] = wing(Df);
		fd.dtip(iteration,index) = (tiptwist - first_tiptwist) / fd.h(iteration);
	end
end

%%the senstivities with respect to the first diameter
for iteration = 1:n_steps
	Df = D;
	Df(1) = Df(1) + fd.h(iteration);
	[tiptwist, u] = wing(Df);
	fd.du(iteration,:) = (u - first_u)./fd.h(iteration);
end
%compute the errors of the finite difference
% A_error = [step_size, numerical partial error]
% A_error = [];
% index = 1;
% for A_partial = f_partials(:,1)'
% 	if(A_partial)
% 		%accumulate in the error vector
% 		A_error(end+1,1) = f_h(index);
% 		A_error(end,2) = abs(A_partial - cs_partials(1));
% 	end
% 	index = index + 1;
% end
% S_error = [];
% index = 1;
% for S_partial = f_partials(:,2)'
% 	if(S_partial)
% 		%accumulate in the error vector
% 		S_error(end+1,1) = f_h(index);
% 		S_error(end,2) = abs(S_partial - cs_partials(2));
% 	end
% 	index = index + 1;
% end

% figure(1)
% loglog(A_error(:,1), A_error(:,2),'r')
% hold on
% loglog(S_error(:,1), S_error(:,2), 'b-.')
% legend('\deltaD/\deltaA | A = 10', '\deltaD/\deltaS | S = 20')
% xlabel('Finite Difference Step Size')
% ylabel('Absolute Numerical Error')
