clear
clc

u = ones(99,1);
y = 1.0;
[f,dfdy, dfdu] = bb5_obj(y, u);

[r, b, A] = pde(y, u);