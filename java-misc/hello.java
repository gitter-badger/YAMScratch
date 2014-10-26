class HelloWorld {
	public static void main(String[] argv){
		System.out.println("HelloWorld"); 
		try{
			System.out.println(HelloWorld.foo(4));
		}
		catch(IndexOutOfBoundsException e){
			System.err.println(e);
		}
		// this is a comment
	}
	private static int foo(int  bar){
		int[] f = new int[2];
		return f[bar];
	}
}