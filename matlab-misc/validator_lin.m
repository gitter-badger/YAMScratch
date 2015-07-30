clear
clc
%K is the length of the password, must by divisible by 2
K = 10;

%there are only 6 bytes determined in output
A = zeros(6, K);
%reduced rank solution vector
b = zeros(6, K);

%first two are every even checksum
b(1,1) = 0; %byte b
b(2,1) = 0; %byte a
%next two are left half checksum
b(3,1) = 208; %byte b
b(4,1) = 110; %byte a
%next two are right half checksum
b(5,1) = 240;
b(6,1) = 13;
%instatiate the matrix
for ii = 1:(K/2)
	%even bytes checksum
	A(1, ii*2 - 1) = K/2 - ii + 1;
	A(2, ii*2 - 1) = 1;
	%left size check sum
	A(3,ii) = K/2 - ii + 1;
	A(4,ii) = 1;
	%right half checksum
	A(5,ii+(K/2)) = K/2 - ii + 1;
	A(6, ii+(K/2)) = 1;
end

disp(A) 

%NOTE, now we just solve this using gauss elimination on Z_bar(255) (finite field)
