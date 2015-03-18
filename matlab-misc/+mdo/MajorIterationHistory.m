%Defines an standard handle for storing information about optimization
%routines for each major iteration of algorithm
classdef MajorIterationHistory < handle
    properties
        total_iterations;
        x;
        g;
    end
    methods
        function obj = MajorIterationHistory()
            obj.total_iterations = 0;
        end
        function editIteration(obj, k, x, g)
            %convert the vector to a column vector
            assert(isvector(x))
            assert(isvector(g))
            if(k > obj.total_iterations)
                obj.total_iterations = k;
            end
            obj.x(k, :) = x;
            obj.g(k, :) = g;
        end
        function appendIteration(obj, x, g)
            obj.total_iterations = obj.total_iterations + 1;
            obj(end+1, :) = x;
            obj(end+1, :) = g;
        end
    end
end
