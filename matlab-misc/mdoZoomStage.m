function [alpha] = mdoZoomStage(phi,al_low,al_high,boundary)
%Purpose: returns
% al_low - the step size for which phi is smaller
% al_high - the step size for which phi is larger
% boundary - a struct containing fields listed below
%		f_low - vector corresponding to phi(al_low)
%		g_low - vector corresponding to phi'(al_low)  is possible to be NaN
%		f_high - vector corresponding to phi(al_high)
%		g_high - vector corresponding to phi(al_high)
%		f_init - vector corresponding to phi(0)
%		g_init - vector corresponding to phi'(0)
disp(boundary_conditions)
	while true
		%find a trial point between al_low and al_high
		%this uses bisection
		if 
		%now zoom into the interval
		[f_this, g_this] = phi(alpha_this);

		return
		%
	end
		return
end	