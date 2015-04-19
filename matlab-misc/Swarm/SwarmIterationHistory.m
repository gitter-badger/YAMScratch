classdef SwarmIterationHistory < handle
	properties
		niterations
		nparticles
		Velocities
		Positions
		Xbest
		Xbestindex
		Xworst
		Xworstindex
		Ybest
		Ybestindex
		Yworst
		Yworstindex
	end
	methods

		function obj = SwarmIterationHistory(niter, npart)
			obj.niterations = niter;
			obj.nparticles = npart;
			return
		end
		function m_lowEditIteration(obj, iteration, Xbest, Ybest, varargin)
		%Usage:
			m_lowEditIteration(iteration, Xbest, Ybest)
			m_lowEditIteration(iteration, Xbest, Ybest, Xworst, Yworst)

			assert(0 < iteration );
			assert(obj.niterations >= iteration);
			assert(isvector(Xbest));
			assert(isscalar(Ybest));
			obj.Xbest(iteration,:) = Xbest;
			obj.Ybest(iteration,1) = Ybest;
			if nargin == 4
				obj.Xworst(iteration,:) = varargin{1};
				obj.Yworst(iteration,1) = varargin{2};
			end
		end
		function m_highEditIteration(obj, iteration, positions, velocities, xbestndx, Ybest )
		end
	end
end