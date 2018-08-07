// InvokeVirtual

class Main
{
    final static int iterations = 11;
    public static void main(String[] args)
    {
        Test test = new Test();
        int c = 0;
        try{
        
           for (int i = 1; i < iterations; i++) {
	        c = test.b(i);
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


