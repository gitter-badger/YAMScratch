function [alpha] = ZoomStage(phi,al_low,al_high,b)
%% [alpha] = ZoomStage(phi,al_low,al_high,b)
%Purpose: returns an acceptable step size
% al_low - the step size for which phi is smaller
% al_high - the step size for which phi is larger
% b - a struct containing boundary conditions in fields listed below
%		f_low - vector corresponding to phi(al_low)
%		g_low - vector corresponding to phi'(al_low)  is possible to be NaN
%		f_high - vector corresponding to phi(al_high)
%		g_high - vector corresponding to phi(al_high)
%		f_init - vector corresponding to phi(0)
%		g_init - vector corresponding to phi'(0)
%		mu_1 - initial condition for sufficient decrease
%		mu_2 - initial condition for curvature decrease
%		p - the direction of search
% Returns:
% alpha - an acceptable step length

	iteration = 1;
	%we limit the iteration count to prevent a degenracy observed
	while iteration < 30
		%find a trial point between al_low and al_high
		%this uses bisection, but something else could be used instead
		a_trial = (al_low + al_high) /2;
		[f_trial, g_trial] = phi(a_trial);
		%this comparsion only works for scalar valued functions
		if (f_trial) > (b.f_init +b.mu_1*a_trial*b.g_init.'*b.p) ...
			|| (f_trial > b.f_low)
			%make interval smaller
			al_high = a_trial;
			b.f_high = f_trial;
			%we are not supposed to know g_trial right now
			b.g_high = NaN;
		else
			%here is where we would evaluate the derivative
			%g_trial = some derivative calc
			%now check if the curvature condition is satisfied
			if (abs(g_trial.'*b.p) <= b.mu_2*abs(b.g_init.'*b.p))
				alpha = a_trial;
				return
			elseif (g_trial*(al_high - al_low) >= 0)
				%replace high point with low point
				al_high = al_low;
				b.f_high = b.f_low;
				b.g_high = b.g_low;
				%make sure these values are not used
				b.f_low = NaN;
				b.g_low = NaN;
				al_low = NaN;
			end
			%replace low point with trial point
			al_low = a_trial;
			b.f_low = f_trial;
			b.g_low = g_trial;
		end
		iteration = iteration + 1;
	end
	%return with the best guess if iteration count is exceeded
	alpha = a_trial;
	return
end	
