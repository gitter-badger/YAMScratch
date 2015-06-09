def main():
	N = int(raw_input())
	result = map(min, [[int(x) for x in raw_input().split(" ")] for _ in range(0,N)])
	print result


if __name__ == "__main__":
	main()
