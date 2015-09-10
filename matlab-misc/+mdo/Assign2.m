clear('all');
clc
close all;

%part 1
A = 10;
S = 20;
%%Complex Step method
cs_h = 1e-60;
reg_drag = mdo.DragTotal(A,S);

cs_partials = [imag(mdo.DragTotal(A+1i*cs_h, S)), ...
				imag(mdo.DragTotal(A, S+1i*cs_h))];
cs_partials = cs_partials / cs_h;

%%Finite difference
%test different step sizes
f_h = logspace(-1,-35,1000);
f_partials = [];
index = 1;
for step = f_h
	f_partials(index,1) = (mdo.DragTotal(A+step, S) - reg_drag)/step;
	f_partials(index,2) = (mdo.DragTotal(A, S+step) - reg_drag)/step;
	index = index + 1;
end

%compute the errors of the finite difference
% A_error = [step_size, numerical partial error]
A_error = [];
index = 1;
for A_partial = f_partials(:,1)'
	if(A_partial)
		%accumulate in the error vector
		A_error(end+1,1) = f_h(index);
		A_error(end,2) = abs(A_partial - cs_partials(1));
	end
	index = index + 1;
end
S_error = [];
index = 1;
for S_partial = f_partials(:,2)'
	if(S_partial)
		%accumulate in the error vector
		S_error(end+1,1) = f_h(index);
		S_error(end,2) = abs(S_partial - cs_partials(2));
	end
	index = index + 1;
end

figure(1)
loglog(A_error(:,1), A_error(:,2),'r')
hold on
loglog(S_error(:,1), S_error(:,2), 'b-.')
legend('\deltaD/\deltaA | A = 10', '\deltaD/\deltaS | S = 20')
xlabel('Finite Difference Step Size')
ylabel('Absolute Numerical Error')
