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
           ex.printStackTrace();
	}
        catch (Exception ex){
          System.out.println("Caught unhandled exception");
          ex.printStackTrace();
       }
    }
}
