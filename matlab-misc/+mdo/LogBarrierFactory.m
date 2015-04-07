function [ff, fg] = LogBarrierFactory(planeObj, V_min, C_L_max)
	function [phi] =logBarrier(A, S, Mu)
		%flip the sign on the stall constraint because it is in form c(x) <= 0
		phi = planeObj.m_DragForce(A, S)  + Mu * (log( -planeObj.m_LandingConstraint(A, S, V_min, C_L_max)));
	end

	function [gradphi] = gradLogBarrier(A, S, Mu)
		[drag,grad_drag] = planeObj.m_DragForce(A,S);
		[cons, grad_cons] = planeObj.m_LandingConstraint(A, S, V_min, C_L_max);
		%flip the sign on the stall constraint because it is in form c(x) <= 0
		gradphi = grad_drag + (Mu * 1/(-planeObj.m_LandingConstraint(A, S, V_min, C_L_max))) * -1 * grad_cons;
	end
	ff = @logBarrier;
	fg = @gradLogBarrier;
	return
end

