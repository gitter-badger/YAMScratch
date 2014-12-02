clear
clc
close all
%populate the data array with fresh values
root_dir = 'Lab 4 data/';
derivative_gain_curry(root_dir);
proportional_gain_curry(root_dir);
integral_gain_curry(root_dir);

load('Pure_Proportional.mat','-mat')
top_level = fieldnames(pure_pro);
%get the time axis
temp_time_axis = pure_pro.(top_level{1}).time;
temp_time_axis = temp_time_axis - temp_time_axis(1,1);
%plot one total cycle
base_P_case = top_level{1};
figure
plot(temp_time_axis,pure_pro.(base_P_case).input,'k:')
hold on
plot(temp_time_axis,pure_pro.(base_P_case).sys_out,'r')
hold on
legend({'Reference Signal','P Controller'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('System Response Compared with Reference Signal')
%plot half cycle for evaluation of SSE
figure
plot(temp_time_axis(1:126),pure_pro.(base_P_case).input(1:126),'k:')
hold on
plot(temp_time_axis(1:126),pure_pro.(base_P_case).sys_out(1:126),'r')
legend_1 = legend({'Reference Signal','P Controller'});
set(legend_1,...
    'Position',[0.64 0.18 0.225 0.15]);
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Evaluation of Steady State Error of System')
%plot the reference signal over half period
figure
plot(temp_time_axis(1:126,1),pure_pro.(top_level{1}).input(1:126,1),'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Effect of Varying Proportional Gain on System Transient Response')
%define the colors we will use for each line
colors = {'r','g','m','c','k','b','g:'};
%create the measure of steady state error
SSE_Pro = [];
for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    hold on
    plot(temp_time_axis(1:126,1), pure_pro.(name).sys_out(1:126,1),char(colors(counter)))
    hold on
    %add this steady state error
    SSE_Pro(counter,1) = pure_pro.(name).input(120,1) - pure_pro.(name).sys_out(120,1);
end
%display the steady state errors
disp('SSE for Pure Proportional')
disp(SSE_Pro)
%filter the names and make them human readable
partial = regexprep(top_level,'_','.');
partial = regexprep(partial,'^kp','K_p = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    complete_legend(lark+1) = partial(lark);
end
legend_P = legend(complete_legend);
set(legend_P,...
    'Position',[0.64 0.18 0.225 0.349206349206349]);

%====================================================================
load('Proportional_Derivative.mat','-mat')
top_level = fieldnames(proportional_derivative);
%get the time axis
temp_time_axis = proportional_derivative.(top_level{1}).time;
temp_time_axis = temp_time_axis - temp_time_axis(1,1);
%plot one total cycle
figure
base_PD_case = top_level{1};
plot(temp_time_axis,pure_pro.(base_P_case).input,'k:')
hold on
plot(temp_time_axis,pure_pro.(base_P_case).sys_out,'r')
hold on %plot the derivative next
plot(temp_time_axis,proportional_derivative.(base_PD_case).sys_out,'b')
hold on
legend({'Reference Signal','P Controller','PD Controller'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('System Response for P and PD Controllers Compared with Reference Signal')
%plot half cycle for evaluation of SSE
figure
plot(temp_time_axis(1:126),proportional_derivative.(base_PD_case).input(1:126),'k:')
hold on
plot(temp_time_axis(1:126),proportional_derivative.(base_PD_case).sys_out(1:126),'r')
legend_2 = legend({'Reference Signal','PD Controller'});
set(legend_2,...
    'Position',[0.64 0.18 0.225 0.15]);
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Evaluation of Steady State Error of System')
%plot the reference signal for one half cycle
figure
plot(temp_time_axis(1:126,1),proportional_derivative.(top_level{1}).input(1:126,1),'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Effect of Varying Derivative Gain on System Transient Response')
SSE_Det = [];
for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    hold on
    plot(temp_time_axis(1:126,1), proportional_derivative.(name).sys_out(1:126,1),char(colors(counter)))
    hold on
    SSE_Det(counter,1) = proportional_derivative.(name).input(120,1) - proportional_derivative.(name).sys_out(120,1);
end
disp('SSE_Det')
disp(SSE_Det)
%filter the names and make them human readable
partial = regexprep(top_level,'_','0.');
partial = regexprep(partial,'^kd','K_d = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    complete_legend(lark+1) = partial(lark);
end
legend_PD = legend(complete_legend);
set(legend_PD,...
    'Position',[0.64 0.18 0.225 0.349206349206349]);

%====================================================================
load('Integral_Derivative.mat','-mat')
top_level = fieldnames(integral_derivative);
%get the time axis
temp_time_axis = integral_derivative.(top_level{1}).time;
temp_time_axis = temp_time_axis - temp_time_axis(1,1);
%plot one total cycle
figure
base_PID_case = top_level{1};
plot(temp_time_axis,pure_pro.(base_P_case).input,'k:')
hold on
plot(temp_time_axis,pure_pro.(base_P_case).sys_out,'r')
hold on %plot the derivative next
plot(temp_time_axis,proportional_derivative.(base_PD_case).sys_out,'g')
hold on % plot the integral
plot(temp_time_axis,integral_derivative.(base_PID_case).sys_out,'b-.')
legend({'Reference Signal','P Controller','PD Controller','PID Controller'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('System Response for P, PD, and PID Controllers Compared with Reference Signal')
%plot half cycle for evaluation of SSE
figure
plot(temp_time_axis(1:126),integral_derivative.(base_PID_case).input(1:126),'k:')
hold on
plot(temp_time_axis(1:126),integral_derivative.(base_PID_case).sys_out(1:126),'r')
legend_3 = legend({'Reference Signal','PID Controller'});
set(legend_3,...
    'Position',[0.64 0.18 0.225 0.15]);
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Evaluation of Steady State Error of System')
%plot the reference signal
figure
plot(temp_time_axis(1:126,1),integral_derivative.(top_level{1}).input(1:126,1),'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Effect of Varying Integral Gain on System Transient Response')
colors = {'r','g','m','k','g:','c','b'};
SSE_Ing = [];
for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    SSE_Ing(counter,1) = integral_derivative.(name).input(120,1) - integral_derivative.(name).sys_out(120,1);

    if mod(counter-1,3) == 0
        hold on
        plot(temp_time_axis(1:126,1), integral_derivative.(name).sys_out(1:126,1),char(colors(counter)))
        hold on
    end
end
disp('SSE_Ing')
disp(SSE_Ing)
%filter the names and make them human readable
partial = regexprep(top_level,'_','');
partial = regexprep(partial,'^ki','K_i = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    if mod(lark-1,3) == 0
        complete_legend(end+1) = partial(lark);
    end
end
legend_PID = legend(complete_legend);
set(legend_PID,...
    'Position',[0.64 0.18 0.225 0.349206349206349]);
