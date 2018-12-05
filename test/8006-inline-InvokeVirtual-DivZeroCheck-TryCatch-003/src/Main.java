// InvokeVirtual

class Main
{
    final static int iterations = 11;

    public static int d(int div) {
        return 1 / div;
    }

    public static void main(String[] args)
    {
        Test test = new Test();
        int c = 0;
        try
	{
	for (int i = 1; i < iterations; i++) {
            try {
                c = d(0);
            } catch (Exception e) {
                // ignore
            } finally {
                c = test.b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
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
