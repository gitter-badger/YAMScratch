clear
clc
%K is the length of the password, must by divisible by 2
K = 10;
MODULUS = 255;

%there are only 6 bytes determined in output
A = zeros(6, K);
%reduced rank solution vector
b = zeros(6, K);

%two are left half checksum
b(1,1) = 110; %byte a == 0x6e
b(2,1) = 208; %byte b == 0xd0
%these two rows every even checksum
%ATTENTION, order of these are swapped
b(3,1) = 0; %byte a == 0x00
b(4,1) = 0; %byte b == 0x00

%next two are right half checksum
b(5,1) = 13; % byte a = 0x0d
b(6,1) = 240; %byte b = 0xf0

%instatiate the matrix
for ii = 1:(K/2)
	%left size check sum
	A(2,ii) = K/2 - ii + 1;
	A(1,ii) = 1;
	%even bytes checksum
	A(3, ii*2 - 1) = 1;
	A(4, ii*2 - 1) = K/2 - ii + 1;


	%right half checksum
	A(6,ii+(K/2)) = K/2 - ii + 1;
	A(5, ii+(K/2)) = 1;
end

disp(A) 

%NOTE, now we just solve this using gauss elimination on Z_bar(255) (finite field)
augmented = A;
augmented(:,K+1)  = b(:,1)

%R2  = R2 - 5*R1
tmp_rw = augmented(2,:) - 5*augmented(1,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(2,:) = tmp_rw(:)
%R3 = R3 - R1
tmp_rw = augmented(3,:) - augmented(1,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(3,:) = tmp_rw(:)
%R4 = R4 - 5*R1
tmp_rw = augmented(4,:) - 5*augmented(1,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(4,:) = tmp_rw

%R2 = R2 * the multiplicative mod inverse of  first element in R2
first_index = find(augmented(2,:));
first_elm = augmented(2,first_index(1));

[d,u,v] = gcd(first_elm, MODULUS);
% if d != 1 then modular inverse does not exist
if d ~= 1
	error('Modular inverse does not exist')
end

tmp_rw = augmented(2,:) * u;
tmp_rw = mod(tmp_rw, MODULUS);
augmented(2, :) = tmp_rw(:)

%R3 =  R3 - R1 * first element in R3
disp('R3 =  R3 - R1 * first element in R3')
first_index = find(augmented(3,:));
first_elm = augmented(3, first_index(1));

tmp_rw = augmented(3,:) - first_elm*augmented(2,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(3,:) = tmp_rw(:)

%R4=  R4 - R2 * first element in R3
disp('R 4=  R4 - R2 * first element in R3')
first_index = find(augmented(4,:));
first_elm = augmented(4, first_index(1));

tmp_rw = augmented(4,:) - first_elm*augmented(2,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(4,:) = tmp_rw(:)

%R3 = R3 * the multiplicative mod inverse of  first element in R3
first_index = find(augmented(3,:));
first_elm = augmented(3,first_index(1));

[d,u,v] = gcd(first_elm, MODULUS);
% if d != 1 then modular inverse does not exist
if d ~= 1
	error('Modular inverse does not exist')
end

tmp_rw = augmented(3,:) * u;
tmp_rw = mod(tmp_rw, MODULUS);
augmented(3, :) = tmp_rw(:)

%R4=  R4 - R3 * first element in R3
disp('R 4=  R4 - R1 * first element in R3')
first_index = find(augmented(4,:));
first_elm = augmented(4, first_index(1));

tmp_rw = augmented(4,:) - first_elm*augmented(3,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(4,:) = tmp_rw(:)

%R6=  R6 - R5 * first element in R6
disp('R 4=  R4 - R1 * first element in R3')
first_index = find(augmented(6,:));
first_elm = augmented(6, first_index(1));

tmp_rw = augmented(6,:) - first_elm*augmented(5,:);
tmp_rw = mod(tmp_rw, MODULUS);
augmented(6,:) = tmp_rw(:)


%R6 = R6 * the multiplicative mod inverse of  first element in R6
first_index = find(augmented(6,:));
first_elm = augmented(6,first_index(1));

[d,u,v] = gcd(first_elm, MODULUS);
% if d != 1 then modular inverse does not exist
if d ~= 1
	error('Modular inverse does not exist')
end

tmp_rw = augmented(6,:) * u;
tmp_rw = mod(tmp_rw, MODULUS);
augmented(6, :) = tmp_rw(:)

%==============================================
% Below here is experimental manipulation of K = 10
%==============================================
if K == 10
	%sweep back on the lower rows
	for jj = 6:-1:2
		first_index = find(augmented(jj,:));
		for ii = (jj-1): -1 : 1
			if augmented(ii, first_index(1)) ~= 0
				%do R_ii
				first_elm = augmented(ii, first_index(1));
				tmp_rw = augmented(ii,:) - first_elm*augmented(jj,:);
				tmp_rw = mod(tmp_rw, MODULUS);
				augmented(ii,:) = tmp_rw(:)

			else
				disp('skipping row')
			end
		end
	end
end
