function [a, b] = challenge_checksum(arry, MODULUS)
	a = 0;
	b = 0;
	for ii = 1:length(arry);
		a = mod(a + arry(ii), MODULUS);
		b = mod(b + a, MODULUS);
	end
	return
end