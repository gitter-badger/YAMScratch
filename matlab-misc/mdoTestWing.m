% test the call to wing.m

clear all;
close all;
format compact;
format long e;

% set the inputs
D = [0.16; 0.06; 0.03; 0.02; 0.02; 0.02];   % tube diameters

cs = struct; %namespace for complex step
fd = struct; %namespace for finite difference
% call wing
[first_tiptwist, first_u] = wing(D);


cs.dtip = zeros(size(D,1),1);

cs.h = 1e-60;
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
%%              Finite difference method             %%
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
legend('\delta\gamma_{tip}/ \deltaD_1', '\delta u_i /\delta D_1')
xlabel('Finite Difference Step Size')
ylabel('Absolute Numerical Error')


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%              Analytic Direct Method               %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%get the original wing deflections
[tiptwist_0, u_0,K] = wing(D);
ndof = size(u_0,2);
BC = zeros(ndof,1);
BC(1:3) = 1;
BC(10:12) = 1;
cs_h = 1e-20;
%next get the partials of the residuals with respect to the design variable
all_dydx = zeros(size(D,1),ndof);
for selector = 1:size(D,1)
    Dc = D;
    Dc(selector) = Dc(selector) + 1i*cs_h;
    drdx(selector,:) = imag(wing2(Dc,u_0)) ./ cs_h;

    %now that the Jac matrix is reduced, solve the linear system
    B = drdx(selector,4:9);
    B = [B drdx(selector,13:ndof)];

    dydx = (K\B.');

    i2 = 1;
    %%more professor code to expand again
        for i = 1 : ndof,
            if BC(i) == 1,
                direct_du(i) = 0;
            else
                direct_du(i) = dydx(i2);
                i2 = i2 + 1;
            end
        end
    %end professor code
    all_dydx(selector,:) = direct_du;
end
%the tip twist partial is just a function of the state variables
[~,dfdy] = wing2(D,u_0);

dfdx = zeros(1,size(D,1));
for index = 1:size(D,1)
    row = all_dydx(index,:);
    dfdx(index) = 0 + dfdy.'*all_dydx(index,:).';
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%              Analytic Adjoint Method              %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%we reuse many variables from the direct method
%solve for psi
BB = dfdy(4:9);
BB = [BB; dfdy(13:ndof)];
%solving for non zero elements of psi
part_psi = -K.'\BB;

%begin professor code to expand
i2 = 1;
for i = 1 : ndof,
        if BC(i) == 1,
            psi(i) = 0;
        else
            psi(i) = part_psi(i2);
            i2 = i2 + 1;
        end
    end
%end professor code

for selector = 1:size(D,1)
    aj_dfdx(selector) = 0 + -psi * drdx(selector,:).';
end
%the state vector sensitivites are computed in the same manner as in direct method

%plot the tip sensitivities
figure(2)
%we find that fd.h(60) is a good step size for the displacements sensitivies
%and fd.h(53) is a good step size for tip sensitivities
fd_du = fd.du(60,:);
fd_dtip = fd.dtip(52,:);

semilogy([1:6], abs(fd_dtip - cs.dtip.'), 'bd')
xlim([0, 7])
hold on
semilogy([1:6], abs(dfdx - cs.dtip.'), 'r+')
hold on
semilogy([1:6], abs(aj_dfdx - cs.dtip.'), 'kd')

l2 = legend('Finite Difference h = 1.5873e-07', 'Analytic Direct Method h = 1e-20', ...
    'Analytic Adjoint Method h = 1e-20');
legend('Location','east')
xlabel('Index (i) of \delta\gamma_{tip} /\delta D_i')
ylabel('Absolute Difference')
title('Comparison of Numerical Gradients to Complex Step Method with h = 10^{-60}')

figure(3)
semilogy([1:ndof], abs(fd_du - cs.du), 'rd')
xlim([1 19])
hold on
semilogy([1:ndof], abs(all_dydx(1,:) - cs.du), 'bd')
hold on

l3 = legend('Finite Difference', 'Analytic Direct Method h = 1e-20');
legend('Location','east')
xlabel('Index (i) of \deltau_i /\delta D_1')
ylabel('Absolute Difference')
title('Comparison of Numerical Gradients to Complex Step Method with h = 10^{-60}')
