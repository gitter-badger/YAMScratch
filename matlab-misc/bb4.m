clear
clc

a = 1.5;
x_step = 0.1*a;
x_lim = -a:x_step:a;
b = 1.5;
y_step = 0.1*b;
y_lim = -b:y_step:b;

C = [];
for indexx = 1:size(x_lim,2)
   disp(indexx)
   for indexy = 1:size(y_lim,2)
       C(indexx,indexy) = cnstr([x_lim(indexx),y_lim(indexy)].');
   end
end

figure
mesh(x_lim,y_lim, C)
xlabel('X')
ylabel('Y')

flat_z = zeros(size(x_lim,2),size(y_lim,2));
zdiff = C - flat_z;

hold on
[Contour,h] = contour(x_lim,y_lim, C, [0,0],'Color', 'k', 'LineWidth', 3);

xL = Contour(1, 2:end);
yL = Contour(1, 2:end);

zL = interp2(x_lim, y_lim, C, xL, yL);