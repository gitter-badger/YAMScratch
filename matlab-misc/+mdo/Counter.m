classdef Counter < handle
	%is a communicator with closures
	properties
		count
	end
	methods
		%constructor
		function self = mdoCounter
			self.count = 0;
			disp('Started counter')
		end
		function increment(self)
			self.count = self.count + 1;
		end
	end
end
