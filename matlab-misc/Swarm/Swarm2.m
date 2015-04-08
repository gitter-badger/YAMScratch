clear
clc
close all
tic
%import com.mathworks.jmi.*
%Matlab.mtEval('sqrt(5)',1)
pwd

file_path = regexprep(mfilename('fullpath'), '(?!\\)[^\\]*$', '')
javaaddpath(file_path)
toc
disp('done')
