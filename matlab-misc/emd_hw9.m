clear
clc
close all

E = 25e9; %effective modulus of shaft
shaft_d = 2e-3; %diameter of shaft in meters
Su = 690e6;	%ultimate strength
Syp = 250e6; %yield strength
shaft_length = 30e-3;
disk_mass = 50e-3; %mass in kilograms
disk_e =  0.01e-3; %ecentricity in meters

allowable_y = 0.04e-3;

shaft_I = pi/4 * (shaft_d/2)^4;

Nc_top_disk = (30/pi)*sqrt((3*E*shaft_I*shaft_length)/ ...
							(disk_mass* shaft_length^4 *(4/81)));

Nc_middle_disk = (30/pi)*sqrt((3*E*shaft_I*shaft_length)/ ...
							(disk_mass* shaft_length^4 /16));

%the top and the bottom disk have the same critical speed
%due to symmetry
Nc_total = (sqrt(2* Nc_top_disk^(-2) + Nc_middle_disk^(-2)))^-1

rpm = 2000:1:12000;
deflection = (disk_e * rpm.^2) ./ abs(Nc_total^2 - rpm.^2);
acceptable_y = deflection;
acceptable_y(acceptable_y > allowable_y) = NaN;
bad_y = find(deflection > allowable_y);
%get the first index where it is above
low_index = min(bad_y) - 1;
%get the last index where it is above
high_index = max(bad_y) + 1;
low_rpm_limit = rpm(low_index)
high_rpm_limit = rpm(high_index)

plot(rpm, deflection, 'b-')
figure
plot(rpm, acceptable_y,'r-')
xlabel('RPM')
ylabel('Deflection (meters)')
