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
        % Purpose:
        %     Change a specific row in each of the logging fields
        % Inputs:
        %     k - the row index we want to change
        %     f - a scalar usually representing the function value
        %     x - a vector of design variables
        %     g - a vector holding the gradient
        %     fevals - a scalar counting number of times the function has been evaluated
        %     defeval - a scalar counting the number of times the gradient has been evaluated
        % Outputs:
        %   None

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
        %Purpose:
        %   Add some information to the end of each column, this is a bad idea because
        %   the columns could be different lengths and currently this is not checked for

            obj.total_iterations = obj.total_iterations + 1;
            obj.x(end+1, :) = x;
            obj.g(end+1, :) = g;
            obj.f(end+1, 1) = f;
            obj.fevals(end+1, 1) = fevals;
            obj.dfevals(end+1, 1) = dfevals;
        end
    end
end
