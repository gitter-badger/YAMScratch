

class FizzBuzz
{
	public static final String batman = "batman";
	public boolean my_flag;
	public static void main (String[] argv)
	{
		for( int i = 0; i <100; i++)
		{

		}
		FizzBuzz some_state = new FizzBuzz(State.WORRIED);
		some_state.PrintTheState();

	}

	State zoop;
	public FizzBuzz(State bar)
	{
		this.zoop = bar;
	}
	private synchronized void PrintTheState()
	{
		switch (zoop)
		{
			case HAPPY:
				System.out.println("State is HAPPY");
				break;
			case SAD:
				System.out.println("State is SAD");
			case WORRIED:
				System.out.println("State is WORRIED");
			case SEVEN:
				System.out.println("State is SEVEN");
		}
		System.out.println((zoop == State.WORRIED) ? "true": "false");
	} 

	public enum State {
		HAPPY, SAD, WORRIED, SEVEN
	}
}