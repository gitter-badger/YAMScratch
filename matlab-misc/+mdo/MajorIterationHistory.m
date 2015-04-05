%Defines an standard handle for storing information about optimization
%routines for each major iteration of algorithm
classdef MajorIterationHistory < handle
    properties
        total_iterations;
        f;
        x;
        g;
        fevals;
        dfevals;
        optimality;
    end
    methods
        function obj = MajorIterationHistory()
            obj.total_iterations = 0;
        end
        function editIteration(obj, k, f, x, g, fevals, dfevals, varargin)
            %convert the vector to a column vector
            assert(isvector(x));
            assert(isvector(g));
            assert(isscalar(f));
            assert(isscalar(fevals));
            assert(isscalar(dfevals));
            if(k > obj.total_iterations)
                obj.total_iterations = k;
            end
            obj.x(k, :) = x;
            obj.g(k, :) = g;
            obj.f(k, 1) = f;
            obj.fevals(k, 1) = fevals;
            obj.dfevals(k, 1) = dfevals;
            if nargin == 8
                obj.optimality(k,1) = varargin{1};
            end
        end
        function appendIteration(obj, x, g, fevals, dfevals)
            obj.total_iterations = obj.total_iterations + 1;
            obj.x(end+1, :) = x;
            obj.g(end+1, :) = g;
            obj.f(end+1, 1) = f;
            obj.fevals(end+1, 1) = fevals;
            obj.dfevals(end+1, 1) = dfevals;
        end
    end
end
