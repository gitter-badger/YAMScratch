#Main script#
=======================
Running from MatLab does not take any command line arguments.
```Matlab
>> mdoBlackBox3
```
There are config options to adjust at the top of the script file.
You can also play with the line search parameters. Note that the mdoCounter and mdoEvalCounter are  just wrappers for the objective
function. The function handle that mdoEvalCounter returns will replace
the objective function and behave identically.

If you write: 
```Matlab
obj = @(a,b,c, ...) ( some expression here)
%or
obj = @my_objective_function
```
You can replace this handle with a call to to mdoEvalCounter after
constructing a counter object that will allow outside access of the 
number of function evaluations.
```Matlab
%create counter object
counter = mdoCounter();
%then replace your objective handle
obj = mdoEvalCounter(@my_objective_function, counter);
%mdoEvalCounter requires you to pass in a function handle
```
Then to see how many time the function has been evaluated.
```Matlab
disp(counter.count)
```


#Dependencies#
================================
	mdoCounter.m
	mdoEvalCounter.m
	mdoLineSearch.m
	mdoZoomStage.m

Objective function:
	You will need a scalar valued objective function that takes a 128 
	vector as input. In my case I am using an encrypted black box 
	function.

	func3.p

#Example runs#
=====================
There are 4 example runs of this code. The starting point for each run 
can be found by slicing the cached line search points:
```Matlab
>> x_metric(1,:)'
```
The gradient vector is also cached and can be accessed identically for 
each line search point.