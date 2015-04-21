clear;
close all
import mdo.*

DEBUG = false;
PLOT_OBJECTIVE = true;
COLLECT_STATS = false;

d = datestr(clock);
filename = regexprep(d, '\s|-|:', '_');

%Define Constants
N_ult = 2.5;
t_over_c = 0.12;
W_0 = 4940;
rho = 1.23;         %density of air
mu = 17.8e-6;       %viscosity of air
k = 1.2;            %form factor
e = 0.96;           %Oswald efficiency factor
S_wet_ratio = 2.05;
Velocity = 35;

V_min = 22;         %maximum landing velocity
C_L_max = 2;        %maximum coefficient of lift at landing

X_0 = [35; 35];
%===========================================================
%The plane object that holds all of the methods
plane = mdo.ComputeAirPlane2(N_ult, t_over_c, W_0, rho, mu, k, e, S_wet_ratio, Velocity);
fig_1 = figure;

if PLOT_OBJECTIVE
    a = [5:1e-1:35]; % A
    s = [5:1e-1:40]; % S
    [AA,SS] = meshgrid(a,s);
    tic
    Drag = arrayfun(@plane.m_DragForce, AA, SS);
    toc
    contour(AA,SS,Drag,50);
    hold on
    %mesh(AA, SS, Drag)
    xlabel('A');
    ylab = ylabel('S','Rotation',0);
    set(ylab,'Units','Normalized','Position',[-0.07 0.5 0]);
    hold on
    disconline = a*0.85^2;
    disconline(disconline < 5) = NaN;
    plot(a,disconline,'r--')
end


%===========================================================
obj = @(X)(plane.m_DragForce(X(1), X(2)));
if COLLECT_STATS
    stop_criteria = [1:50];
    num_samples = 200;
    fvals_accum = zeros(length(stop_criteria), num_samples);
    run_times = zeros(length(stop_criteria), num_samples);

    for nnn = 1:length(stop_criteria)
        for ndata = 1:num_samples
            tstart = tic;
            flag_1 = true;
            try
            [x_star, fval] = Swarm3(obj, [5;5], [40;40], stop_criteria(nnn));
            catch ME
                flag_1 = false;
            end
            run_times(nnn, ndata) = toc(tstart);
            if flag_1
                fvals_accum(nnn, ndata) = fval;
            else
                fvals_accum(nnn, ndata) = NaN;
            end
        end
        disp('===============================')
    end
    save(filename, 'fvals_accum', 'S_devs', 'f_means', 'run_times', 'stop_criteria')
else
    while true
        filename = input('Enter filename with pso statistics to load: ', 's')
        response = input(strcat('Do you want to load: ', filename, ' [N/y]? '), 's');
        if response == 'y'
            try
                load(filename);
                break
            catch ME
                disp('could not load file, try again')
            end
            
        end
    end
    [x_star, fval] = Swarm3(obj, [5;5], [40;40]);
end
%rows are convergence criteria, columns are individual tests
[r1,c1] = size(fvals_accum);

%std computes column wise and our samples are row wise so transpose
S_devs = std(fvals_accum.');
f_means = mean(fvals_accum, 2).';
%plot the best of each values as well
best_vals = min(fvals_accum, [], 2).';
worst_vals = max(fvals_accum, [], 2).';
overall_best_val = min(best_vals);

time_S_devs = std(run_times.');
time_means = mean(run_times.');
%generate mean squares for each run
gev_coefficients = zeros(r1, 3);
for index = 1:r1
    gev_coefficients(index, :) =  mle(fvals_accum(index,:),'distribution', 'gev');
end

compare_gev_mean_to_best_fig = figure;
semilogy(1:50, gev_coefficients(:,3) - overall_best_val,'rd')
xlabel('Exit condition (iterations)');
ya1 = ylabel('$\Delta_{mean}$', 'interpreter', 'latex','Rotation',0, 'FontSize', 16);
set(ya1,'Units','Normalized','Position',[-0.10 0.55 0]);

low_bound_gev_fig = figure;
low_bound_gev = gev_coefficients(:,3) - (gev_coefficients(:,2) ./ gev_coefficients(:,1));

semilogy(1:50,  overall_best_val - low_bound_gev.', 'kd')
xlabel('Exit condition (iterations)');
ya1 = ylabel('$\Delta_{lower}$', 'interpreter', 'latex','Rotation',0, 'FontSize', 16);
set(ya1,'Units','Normalized','Position',[-0.10 0.5 0]);

% data_fig = figure;
% errorbar(stop_criteria, gev_coefficients(:,3), gev_coefficients(:,1))
% hold on
% plot(stop_criteria, best_vals, 'g-')
% hold on
% plot(stop_criteria, worst_vals, 'r-')

time_fig = figure;
errorbar(stop_criteria, time_means, time_S_devs)
xlabel('Termination Criteria')
ylabel('Runtime (seconds)')

plott_fig = figure;

NUM_LINSPACE = 1500;

first_time_fig = subplot(3,1,1);
selector = 1;
[N,bincntr] = hist(fvals_accum(selector,:),50);
hist(fvals_accum(selector,:),50)
binWidth = bincntr(2) - bincntr(1);
hold on
 fvals_accum(selector,:);
A_gev = gev_coefficients(selector,1);
B_gev = gev_coefficients(selector,2);
C_gev = gev_coefficients(selector,3);
max_val = max(fvals_accum(selector,:));
temp_vals(1,:) = linspace(low_bound_gev(selector), max_val,NUM_LINSPACE);
temp_vals(2,:) = pdf('gev',temp_vals(1,:), A_gev, B_gev, C_gev);
[ax, p1, p2] = plotyy(temp_vals(1,:), zeros(1,NUM_LINSPACE),temp_vals(1,:), temp_vals(2,:), 'plot', 'plot');
delete(p1)
%recolor the bloody axis
set(ax(1), 'YColor', 'k')
set(ax(2), 'YColor', 'r')
set(p2,'Color', 'r')
set(p2, 'LineWidth', 2)
legend('200 samples, 50 bins','GEV estimator')
title(strcat('Termination Criteria = ',' ', num2str(selector)))

x_low_bound = gev_coefficients(selector,3) - 2*(gev_coefficients(selector,2)./gev_coefficients(selector,1));
x_high_bound = gev_coefficients(selector,3) + 7*binWidth;
set(ax(1), 'XLim', [x_low_bound, x_high_bound])
set(ax(2), 'XLim', [x_low_bound, x_high_bound])

%====================================================
third_time_fig = subplot(3,1,2);
selector = 3;
[N,bincntr] = hist(fvals_accum(selector,:),50);
hist(fvals_accum(selector,:),50)
binWidth = bincntr(2) - bincntr(1);
hold on
 fvals_accum(selector,:);
A_gev = gev_coefficients(selector,1);
B_gev = gev_coefficients(selector,2);
C_gev = gev_coefficients(selector,3);
max_val = max(fvals_accum(selector,:));
temp_vals(1,:) = linspace(low_bound_gev(selector), max_val,NUM_LINSPACE);
temp_vals(2,:) = pdf('gev',temp_vals(1,:), A_gev, B_gev, C_gev);
[ax, p1, p2] = plotyy(temp_vals(1,:), zeros(1,NUM_LINSPACE),temp_vals(1,:), temp_vals(2,:), 'plot', 'plot');
delete(p1)
%recolor the bloody axis
set(ax(1), 'YColor', 'k')
set(ax(2), 'YColor', 'r')
set(p2,'Color', 'r')
set(p2, 'LineWidth', 2)
legend('200 samples, 50 bins','GEV estimator')
title(strcat('Termination Criteria = ',' ', num2str(selector)))

x_low_bound = gev_coefficients(selector,3) - 2*(gev_coefficients(selector,2)./gev_coefficients(selector,1));
x_high_bound = gev_coefficients(selector,3) + 7*binWidth;
set(ax(1), 'XLim', [x_low_bound, x_high_bound])
set(ax(2), 'XLim', [x_low_bound, x_high_bound])

%====================================================
fifth_time_fig = subplot(3,1,3);
selector = 5;
[N,bincntr] = hist(fvals_accum(selector,:),50);
hist(fvals_accum(selector,:),50)
binWidth = bincntr(2) - bincntr(1);
hold on
 fvals_accum(selector,:);
A_gev = gev_coefficients(selector,1);
B_gev = gev_coefficients(selector,2);
C_gev = gev_coefficients(selector,3);
max_val = max(fvals_accum(selector,:));
temp_vals(1,:) = linspace(low_bound_gev(selector), max_val,NUM_LINSPACE);
temp_vals(2,:) = pdf('gev',temp_vals(1,:), A_gev, B_gev, C_gev);
[ax, p1, p2] = plotyy(temp_vals(1,:), zeros(1,NUM_LINSPACE),temp_vals(1,:), temp_vals(2,:), 'plot', 'plot');
delete(p1)
%recolor the bloody axis
set(ax(1), 'YColor', 'k')
set(ax(2), 'YColor', 'r')
set(p2,'Color', 'r')
set(p2, 'LineWidth', 2)
legend('200 samples, 50 bins','GEV estimator')
title(strcat('Termination Criteria = ',' ', num2str(selector)))

x_low_bound = gev_coefficients(selector,3) - 2*(gev_coefficients(selector,2)./gev_coefficients(selector,1));
x_high_bound = gev_coefficients(selector,3) + 7*binWidth;
set(ax(1), 'XLim', [x_low_bound, x_high_bound])
set(ax(2), 'XLim', [x_low_bound, x_high_bound])


%===========================================================
%   Visualize the swarm moving
%===========================================================

%make sure we go full 300 iterations
[x_star, fval, swarmlog1] = Swarm3(obj, [5;5], [40;40], 300, 'high');

swarm_progress_fig = figure;
%#################
subplot(2,2,1)
if PLOT_OBJECTIVE
    contour(AA,SS,Drag,50);
    hold on
    plot(a,disconline,'r--')
    hold on
end
iteration = 1;
plot(swarmlog1.Positions(1,:,iteration), swarmlog1.Positions(2,:,iteration), 'rd')
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.1 0.5 0]);
title('Iteration = 1')
%#################
subplot(2,2,2)
iteration = 10;
if PLOT_OBJECTIVE
    contour(AA,SS,Drag,50);
    hold on
    plot(a,disconline,'r--')
    hold on
end
plot(swarmlog1.Positions(1,:,iteration), swarmlog1.Positions(2,:,iteration), 'rd');
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.1 0.5 0]);
title('Iteration = 10')
%#################
subplot(2,2,3)
iteration = 50;
%zoom in here so recompute the contour
if PLOT_OBJECTIVE
    a = linspace(17, 17.5); % A
    s = linspace(12.35, 12.55); % S
    [AA,SS] = meshgrid(a,s);
    tic
    Drag = arrayfun(@plane.m_DragForce, AA, SS);
    toc
    contour(AA,SS,Drag,50);
    hold on
    disconline = a*0.85^2;
    disconline(disconline < 5) = NaN;
    plot(a,disconline,'r--')
end
plot(swarmlog1.Positions(1,:,iteration), swarmlog1.Positions(2,:,iteration), 'rd');
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.1 0.5 0]);
title('Iteration = 50')
%#################
subplot(2,2,4)
iteration = 300;
if PLOT_OBJECTIVE
    a = linspace(17, 17.5); % A
    s = linspace(12.35, 12.55); % S
    [AA,SS] = meshgrid(a,s);
    tic
    Drag = arrayfun(@plane.m_DragForce, AA, SS);
    toc
    contour(AA,SS,Drag,50);
    hold on
    disconline = a*0.85^2;
    disconline(disconline < 5) = NaN;
    plot(a,disconline,'r--')
    hold on
end
plot(swarmlog1.Positions(1,:,iteration), swarmlog1.Positions(2,:,iteration), 'rd');
xlabel('A');
ylab = ylabel('S','Rotation',0);
set(ylab,'Units','Normalized','Position',[-0.1 0.5 0]);
title('Iteration = 300')
%#################

%===========================================================
%               BFGS SEARCH
%===========================================================
figure(fig_1)
plot(x_star(1), x_star(2), 'kd')

linesearch = @mdo.ProfLinesearch;
%configure the linesearch parameters here
mu_1 = 1e-4;
mu_2 = 0.9;
alpha_init = .5;
alpha_max = 10;
tolerance = 5e-5;
ls_parameters = [mu_1, mu_2, alpha_init, alpha_max, tolerance];
e_g = 1e-6;
e_a = 1e-6;
e_r = 1e-6;

grad = @(X) (plane.mH_gradDragForce(X(1), X(2)));

qn_log1 = MajorIterationHistory();
X_1 = X_0;
try
    [x1, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_1, e_g, e_a, e_r, qn_log1, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end
qn_log2 = MajorIterationHistory();
X_2 = [10;30];
try
    [x2, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_2, e_g, e_a, e_r, qn_log2, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end
qn_log3 = MajorIterationHistory();
X_3 = [24;7];
try
    [x3, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_3, e_g, e_a, e_r, qn_log3, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end
qn_log4 = MajorIterationHistory();
X_4 = [30;20];
try
    [x4, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_4, e_g, e_a, e_r, qn_log4, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end
qn_log5 = MajorIterationHistory();
X_5 = [23;23];
try
    [x5, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_5, e_g, e_a, e_r, qn_log5, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end
qn_log6 = MajorIterationHistory();
X_6 = [13;6];
try
    [x5, hessian] = QuasiNewtonBFGS(linesearch, obj, grad, X_6, e_g, e_a, e_r, qn_log6, ls_parameters);
    plot(xk(1),xk(2),'rd')
catch ME
end

%plot the convergence of the algorithm for several points
hold on 
plot(qn_log1.x(:,1), qn_log1.x(:,2), 'kd-', 'LineWidth', 2)
hold on
plot(qn_log2.x(:,1), qn_log2.x(:,2), 'rd-', 'LineWidth', 2)
hold on
plot(qn_log3.x(:,1), qn_log3.x(:,2), 'bd-', 'LineWidth', 2)
hold on
plot(qn_log4.x(:,1), qn_log4.x(:,2), 'md-', 'LineWidth', 2)
hold on
plot(qn_log5.x(:,1), qn_log5.x(:,2), 'cd-', 'LineWidth', 2)
hold on
plot(qn_log6.x(:,1), qn_log6.x(:,2), 'gd-', 'LineWidth', 2)
