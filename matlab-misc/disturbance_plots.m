clear
clc
root_dir = 'Lab 4 data/';

kd_files = {'kd_03','kd_05','kd_07','kd_09','kd_12'};
ki_files = {'ki1','ki2','ki3','ki4','ki5','ki6'};
kp_files = {'kp1_41','kp2_41','kp3_41','kp4_41','kp15'};
root_name = 'det0';
KD = struct; %container for all derivative gains
KP = struct;
KI = struct;
all_files = {kd_files,ki_files,kp_files};
for group = 1:length(all_files)
    target = struct;
    file_list = all_files{group};
    disp('Processing...')
    for i = 1:length(file_list)
        filename = strcat(root_dir,root_name, file_list{i},'.mat');
        disp(strjoin({'    ',filename}))
        temp = struct;
        names = who('-file',filename); %get the variable names from the file
        NewData = load(filename,'-mat'); %get the acutal variables
        for j=1:length(names)
            var_name = names(j); % get the name
            %disp(var_name)
            match = regexp(var_name,'Time','match'); %get cell array with rexex match
            if strcmp(match{1,1},'Time')
                time_var_name = var_name; %figure out the time variable name
            else
                match = regexp(var_name,'Value','match');
                if strcmp(match{1,1},'Value')
                    val_var_name = var_name; %figure out the value variable name
                end
            end
        end
        time = NewData.(char(time_var_name)); %store the time arrays
        signals = NewData.(char(val_var_name)); % store the signals
        %take out the individual signals
        sys_out = signals(:,1); %actual system response
        reference = signals(:,2); %reference signal
        temp = setfield(temp,'input',reference);
        temp = setfield(temp,'sys_out',sys_out);
        temp = setfield(temp,'time',time);
        target = setfield(target,file_list{i},temp);
        clear NewData %clean up
    end
    if group == 1
        KD = target;
    elseif group == 2
        KI = target;
    elseif group == 3
        KP = target;
    end
end
clear reference sys_out i j time_var_name val_var_name signals filename
clear temp time var_name match names group target
close all
time = KD.kd_03.time;
time = time - time(2308);
KD.kd_03.cor_time = time(2308:2436);
KD.kd_03.cor_out = KD.kd_03.sys_out(2308:2436);
plot(KD.kd_03.cor_time(1:100),KD.kd_03.cor_out(1:100),'r')
hold on 

time = KD.kd_05.time;
time = time - time(3269);
KD.kd_05.cor_time = time(3269:3461);
KD.kd_05.cor_out = KD.kd_05.sys_out(3269:3461);
plot(KD.kd_05.cor_time(1:100),KD.kd_05.cor_out(1:100),'b')
hold on

time = KD.kd_07.time;
time = time - time(3365);
KD.kd_07.cor_time = time(3365:3500);
KD.kd_07.cor_out = KD.kd_07.sys_out(3365:3500);
plot(KD.kd_07.cor_time(1:100),KD.kd_07.cor_out(1:100),'g')
hold on

time = KD.kd_09.time;
time = time - time(3778);
KD.kd_09.cor_time = time(3778:3915);
KD.kd_09.cor_out = KD.kd_09.sys_out(3778:3915);
plot(KD.kd_09.cor_time(1:100),KD.kd_09.cor_out(1:100),'m')
hold on

time = KD.kd_12.time;
time = time - time(2137);
KD.kd_12.cor_time = time(2137:2294);
KD.kd_12.cor_out = KD.kd_12.sys_out(2137:2294);
plot(KD.kd_12.cor_time(1:100),KD.kd_12.cor_out(1:100),'k')
hold on

legend({'K_d = 0.03','K_d = 0.05','K_d = 0.07','K_d = 0.09','K_d = 0.12'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Varying Derivative Gain (K_d) Effect on Disturbance Rejection')
%============================================================
figure
time = KI.ki1.time;
time = time - time(4093);
KI.ki1.cor_time = time(4093:4402);
KI.ki1.cor_out = KI.ki1.sys_out(4093:4402);
plot(KI.ki1.cor_time(1:150),KI.ki1.cor_out(1:150),'r')
hold on

time = KI.ki2.time;
time = time - time(1450);
KI.ki2.cor_time = time(1450:1690);
KI.ki2.cor_out = KI.ki2.sys_out(1450:1690);
plot(KI.ki2.cor_time(1:150),KI.ki2.cor_out(1:150),'b')
hold on

time = KI.ki3.time;
time = time - time(1450);
KI.ki3.cor_time = time(1450:1564);
KI.ki3.cor_out = KI.ki3.sys_out(1450:1564);
plot(KI.ki3.cor_time(1:end),KI.ki3.cor_out(1:end),'m')
hold on

time = KI.ki4.time;
time = time - time(1538);
KI.ki4.cor_time = time(1538:1677);
KI.ki4.cor_out = KI.ki4.sys_out(1538:1677);
plot(KI.ki4.cor_time(1:end),KI.ki4.cor_out(1:end),'g')
hold on

time = KI.ki5.time;
time = time - time(3528);
KI.ki5.cor_time = time(3528:3701);
KI.ki5.cor_out = KI.ki5.sys_out(3528:3701);
plot(KI.ki5.cor_time(1:150),KI.ki5.cor_out(1:150),'c')
hold on

time = KI.ki6.time;
time = time - time(3267);
KI.ki6.cor_time = time(3267:3426);
KI.ki6.cor_out = KI.ki6.sys_out(3267:3426);
plot(KI.ki6.cor_time(1:150),KI.ki6.cor_out(1:150),'k')
hold on

legend({'K_i = 1','K_i = 2','K_i = 3','K_i = 4','K_i = 5','K_i = 6'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Varying Integral Gain (K_i) Effect on Disturbance Rejection')
%============================================================
figure
time = KP.kp1_41.time;
time = time - time(1174);
KP.kp1_41.cor_time = time(1174:1389);
KP.kp1_41.cor_out = KP.kp1_41.sys_out(1174:1389);
plot(KP.kp1_41.cor_time(1:end),KP.kp1_41.cor_out(1:end),'r')
hold on

time = KP.kp2_41.time;
time = time - time(3069);
KP.kp2_41.cor_time = time(3069:3294);
KP.kp2_41.cor_out = KP.kp2_41.sys_out(3069:3294);
plot(KP.kp2_41.cor_time(1:end),KP.kp2_41.cor_out(1:end),'b')
hold on

time = KP.kp3_41.time;
time = time - time(4005);
KP.kp3_41.cor_time = time(4005:4191);
KP.kp3_41.cor_out = KP.kp3_41.sys_out(4005:4191);
plot(KP.kp3_41.cor_time(1:end),KP.kp3_41.cor_out(1:end),'m')
hold on

time = KP.kp4_41.time;
time = time - time(3772);
KP.kp4_41.cor_time = time(3772:3897);
KP.kp4_41.cor_out = KP.kp4_41.sys_out(3772:3897);
plot(KP.kp4_41.cor_time(1:end),KP.kp4_41.cor_out(1:end),'g')
hold on

time = KP.kp15.time;
time = time - time(2623);
KP.kp15.cor_time = time(2623:2846);
KP.kp15.cor_out = KP.kp15.sys_out(2623:2846);
plot(KP.kp15.cor_time(1:end),KP.kp15.cor_out(1:end),'k')
hold on

legend({'K_p = 1.41','K_p = 2.41','K_p = 3.41','K_p = 4.41','K_p = 15'})
xlabel('Time (seconds)')
ylabel('Position (radians)')
title('Varying Proportional Gain (K_p) Effect on Disturbance Rejection')