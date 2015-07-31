clear
clc
%K is the length of the password, must by divisible by 2
K = 10;
assert(mod(K, 2) == 0);
assert(K >= 10);
MODULUS = 255;

%these are the character values possible
POSSIBLE_VALS = [65:90, 97:122];

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
%% some tests that the matrix is of same form as checksum

%test for 100 random passwords
tic
for ii = 1:10000
	% draw some random indices from the range
	r = randi([1, length(POSSIBLE_VALS)], 1, K);
	%get the indices and generate the password
	pass_1 = POSSIBLE_VALS(r);

	even_1 = pass_1([1:2:length(pass_1)]);
	left_1 = pass_1(1:(K/2));
	right_1 = pass_1((K/2)+1:K);

	[left_A_1, left_B_1] = challenge_checksum(left_1, MODULUS);
	[right_A_1, right_B_1] = challenge_checksum(right_1, MODULUS);
	[even_A_1, even_B_1] = challenge_checksum(even_1, MODULUS);
	%assemble into the same form as the matrix rows are arranged
	cksum_B = [left_A_1, left_B_1, even_A_1, even_B_1, right_A_1, right_B_1].';


	guess_B = mod(A * pass_1.', MODULUS);
	assert( isequal(guess_B, cksum_B ))
end
toc
disp('Matrix is equivalent to checksums')


%NOTE, now we just solve this using gauss elimination on Z_bar(255) (finite field)
augmented = A;
augmented(:,K+1)  = b(:,1)

for ii = 1:6
	disp(ii)
	first_index = find(augmented(ii,:));
	if augmented(ii, first_index(1)) ~= 1
		first_elm = augmented(ii,first_index(1));
		[d,u,v] = gcd(first_elm, MODULUS);
		% if d != 1 then modular inverse does not exist
		if d ~= 1
			error('Modular inverse does not exist')
		end
		tmp_rw = augmented(ii,:) * u;
		tmp_rw = mod(tmp_rw, MODULUS);
		augmented(ii, :) = tmp_rw(:)
	end
	if ii < 6
		%sweep down
		for jj = (ii+1):6
			first_elm = augmented(jj, first_index(1));
			tmp_rw = augmented(jj,:) - first_elm * augmented(ii,:);
			tmp_rw = mod(tmp_rw, MODULUS);
			augmented(jj,:) = tmp_rw(:)
		end
	end

end
disp('========================')
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

%%extract the reduced matrix and test that it is equivalent to the check sum



%compute with letters are determined by the others
% the determined letters are the columns with a leading 1
determined_letter_index = zeros(1,6);
for ii = 1:6
	first_index = find(augmented(ii,:));
	determined_letter_index(ii) = first_index(1);
end

all_indices = 1:K;

undetermined_indices = setdiff(all_indices, determined_letter_index);

if K == 10
	byte_functions = {};
	for ii = 1:6
		%get the coefficients for the function for each determined letter
		each = augmented(ii, undetermined_indices);
		%get constant
		each_cons = augmented(ii, end);
		%move to other size of equation
		each = mod(-1*each, MODULUS);
		byte_functions{ii} = @(X)(mod((X * each.' + each_cons), MODULUS));
		comparision_vals = [65, 66, 67, 68];
		compare = mod((each * comparision_vals.' + each_cons),MODULUS);
		assert(compare == byte_functions{ii}(comparision_vals));
	end
	%brute force iterate over all possible combinations
	global_count = 0
	valid_solutions = [];
	num_solutions = 0;
	most_valid = 0;
	for ii = 1:length(POSSIBLE_VALS)
		for jj = 1:length(POSSIBLE_VALS)
			for kk = 1:length(POSSIBLE_VALS)
				for ll = 1:length(POSSIBLE_VALS)
					global_count = global_count + 1;
					if mod(global_count,10000) == 0
						disp(global_count./10000)
					end
					guess_chars = [POSSIBLE_VALS(ii), POSSIBLE_VALS(jj), POSSIBLE_VALS(kk), POSSIBLE_VALS(ll)];
					%disp(guess_chars)
					%evaluate each of the functions in turn
					valid_flag = 1;
					for ff = 1:6
						tmp_result = byte_functions{ff}(guess_chars);
						if tmp_result < 65
							valid_flag = 0;
							break;
						end
						if tmp_result > 122
						 	valid_flag = 0;
						 	break;
						end
						if tmp_result < 97
						 	if tmp_result > 90
						 		valid_flag = 0;
						 		break;
						 	end
						end
					end
					if ff >= most_valid
						fprintf('Most valid %d\n', ff);
						disp(guess_chars)
						most_valid = ff;
					end
					if valid_flag == 1
						valid_solutions(end+1, :) = guess_chars;
						num_solutions = num_solutions + 1;
					end
				end
			end
		end
	end
	toc
end

