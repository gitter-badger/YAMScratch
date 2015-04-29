classdef MyCOProblem < handle
	properties
		ndesign;
		CY_all;
	end

	methods
		function obj =  MyCOProblem(nproblems,ndesign)
			obj.ndesign = ndesign;
			obj.CY_all = repmat(linspace(0,10,ndesign).', 1, nproblems);
			return
		end

		function out = getCoeffArray(obj, k)
			out = obj.CY_all(:,k);
			return
		end
	end
end