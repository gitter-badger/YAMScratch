clear
clc
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
%plot the reference signal
plot(temp_time_axis,pure_pro.(top_level{1}).input,'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')
%define the colors we will use for each line
colors = {'r','g','m','c','k','b','g:'};
for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    hold on
    plot(temp_time_axis, pure_pro.(name).sys_out,char(colors(counter)))
    hold on
end
%filter the names and make them human readable
partial = regexprep(top_level,'_','.');
partial = regexprep(partial,'^kp','K_p = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    complete_legend(lark+1) = partial(lark);
end
legend(complete_legend)

%====================================================================
figure
load('Proportional_Derivative.mat','-mat')
top_level = fieldnames(proportional_derivative);
%get the time axis
temp_time_axis = proportional_derivative.(top_level{1}).time;
temp_time_axis = temp_time_axis - temp_time_axis(1,1);
%plot the reference signal
plot(temp_time_axis,proportional_derivative.(top_level{1}).input,'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')

for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    hold on
    plot(temp_time_axis, proportional_derivative.(name).sys_out,char(colors(counter)))
    hold on
end
%filter the names and make them human readable
partial = regexprep(top_level,'_','0.');
partial = regexprep(partial,'^kd','K_d = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    complete_legend(lark+1) = partial(lark);
end
legend(complete_legend)

%====================================================================
figure
load('Integral_Derivative.mat','-mat')
top_level = fieldnames(integral_derivative);
%get the time axis
temp_time_axis = integral_derivative.(top_level{1}).time;
temp_time_axis = temp_time_axis - temp_time_axis(1,1);
%plot the reference signal
plot(temp_time_axis,integral_derivative.(top_level{1}).input,'k:')
legend('Reference Signal')
xlabel('Time (seconds)')
ylabel('Position (radians)')

for counter = 1:length(top_level)
    name = top_level(counter);
    name = char(name);
    hold on
    plot(temp_time_axis, integral_derivative.(name).sys_out,char(colors(counter)))
    hold on
end
%filter the names and make them human readable
partial = regexprep(top_level,'_','');
partial = regexprep(partial,'^ki','K_i = ');
complete_legend = {'Input Signal'};
%add all of the line names to the legend after the reference signal
for lark = 1:length(partial)
    complete_legend(lark+1) = partial(lark);
end
legend(complete_legend)
