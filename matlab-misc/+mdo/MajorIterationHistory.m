%Defines an standard handle for storing information about optimization
%routines for each major iteration of algorithm
classdef MajorIterationHistory < handle
    properties
        total_iterations;
        x;
        g;
        fevals
        dfevals
    end
    methods
        function obj = MajorIterationHistory()
            obj.total_iterations = 0;
        end
        function editIteration(obj, k, x, g, fevals, dfevals)
            %convert the vector to a column vector
            assert(isvector(x))
            assert(isvector(g))
            assert(isscalar(fevals))
            assert(isscalar(dfevals))
            if(k > obj.total_iterations)
                obj.total_iterations = k;
            end
            obj.x(k, :) = x;
            obj.g(k, :) = g;
            obj.fevals(k, 1) = fevals;
            obj.dfevals(k, 1) = dfevals;
        end
        function appendIteration(obj, x, g, fevals, dfevals)
            obj.total_iterations = obj.total_iterations + 1;
            obj(end+1, :) = x;
            obj(end+1, :) = g;
            obj.fevals(end+1, 1) = fevals;
            obj.dfevals(end+1, 1) = dfevals;
        end
    end
end
