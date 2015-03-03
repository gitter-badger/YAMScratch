clear
clc

a = 10;
x_step = 1;
x_lim = -a:x_step:a;
b = 10;
y_step = 1;
y_lim = -b:y_step:b;

C = [];
for indexx = 1:size(x_lim,2)
   disp(indexx)
   for indexy = 1:size(y_lim,2)
       C(indexx,indexy) = cnstr([x_lim(indexx),y_lim(indexy)].');
   end
end

figure(1)
mesh(x_lim,y_lim, C)