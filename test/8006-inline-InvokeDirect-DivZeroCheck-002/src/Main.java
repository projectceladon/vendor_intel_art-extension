// InvokeDirect

class Main
{
    final static int iterations = 11;
    public static void main(String[] args)
    {
        Test test = new Test();
        try{
	   test.a(iterations);
  	}
	catch (ArithmeticException ex){
	System.out.println("Caught expected ArithematicException - " + ex.getMessage());
	}
	catch (Exception ex){
	 System.out.println("Caught unhandled exception");
	}	 
    }
}


