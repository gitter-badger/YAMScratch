clear
clc
close all;
import mdo.*;

obj = @(X,Y)( func6([X,Y].') );

a= 1000;
b= 1000;
grid_scale = 1e2;
x = -a:grid_scale:a;
y = -b:grid_scale:b;

[X,Y] = meshgrid(x,y);
tic
out = arrayfun(obj, X, Y);
toc
%create the filename to save the figure to
d = datestr(clock);
prefix = 'meshplot_';
filename = strcat(prefix, d);
filename = regexprep(filename, '\s|-|:', '_')

mesh_fig = figure;
mesh(x, y, out);
saveas(mesh_fig, filename, 'fig')
disp('All done')
