def _fib():
	fib_memo = [0,1]
	def fib_n(num):
		if(num not in fib_memo):
			while(num not in fib_memo):
				fib_memo.append(fib_memo[-2] + fib_memo[-1])
		return fib_memo.index(num)
	return fib_n

def main():
	fib = _fib()
	N = int(raw_input(""))
	results = []
	for _ in range(0,N):
		target = int(raw_input(""))
		index = fib(target)
		results.append(str(index))
	del fib
	print " ".join(results)

if __name__ == '__main__':
	main()
