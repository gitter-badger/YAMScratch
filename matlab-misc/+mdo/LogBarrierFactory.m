function [ff, fg] = LogBarrierFactory(planeObj, V_min, C_L_max)
	function [phi] =logBarrier(A,S)
		mu = 1;
		%flip the sign on the stall constraint because it is in form c(x) <= 0
		phi = planeObj.m_DragForce(A, S)  + mu * (log( -planeObj.m_LandingConstraint(A, S, V_min, C_L_max)));
	end

	function [gradphi] = gradLogBarrier(A,S)
		[drag,grad_drag] = planeObj.m_DragForce(A,S)

		gradphi = grad_drag + mu * 1/(-planeObj.m_LandingConstraint(A, S, V_min, C_L_max))
	end
	ff = @logBarrier;
	fg = @gradLogBarrier;
	return
end

