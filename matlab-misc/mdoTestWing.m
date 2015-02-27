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
%A_error = [step_size, numerical partial error]
tip_error = [];
selector = 1;
for index = 1:size(fd.dtip,1)
	if(fd.dtip(index,selector))
		%accumulate in the error vector
		tip_error(end+1,1) = fd.h(index);
		tip_error(end,2) = abs(fd.dtip(index,selector) - cs.dtip(selector));
	end
end
u_error = [];

u_selector = 4;
for index = 1:size(fd.du,1)
	if(fd.du(index,u_selector))
		%accumulate in the error vector
		u_error(end+1,1) = fd.h(index);
		u_error(end,2) = abs(fd.du(index,u_selector) - cs.du(u_selector));
	end
end

figure(1)
loglog(tip_error(:,1), tip_error(:,2),'r')
hold on
loglog(u_error(:,1), u_error(:,2), 'b-.')
legend('\delta\gamma_{tip}/\deltaD_1', '\deltau_i/\deltaD_1')
xlabel('Finite Difference Step Size')
ylabel('Absolute Numerical Error')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%				Analytic Direct Method 				 %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%get the original wing deflections
[tiptwist_0, u_0] = wing(D);
ndof = size(u_0,2);
cs_h = 1e-20;
%compute the jacobian of the Residual with respect to y
%one column at time using the complex step
A = [];
for index = 1:ndof
	uc = u_0;
	uc(index) = uc(index) + 1i*cs_h;
	A(:,index) = imag(wing2(D,uc)) ./ cs_h;
end
%next get the partials of the residuals with respect to the design variable
selector = 1;
Dc = D;
Dc(selector) = Dc(selector) + 1i*cs_h;
drdx = imag(wing2(Dc,u_0)) ./ cs_h;
%compute the solution

result = A\drdx.';