// InvokeStatic

public class Main
{
    public static int b(int div) {
        if (div == 10) {
            div = 0;
        }
        int d = 1 / div;
        return d;
    }

    public static int d(int div) {
        return 1 / div;
    }

    public void test() {
        int c = 0;
        for (int i = 1; i < 11; i++) {
            try {
                c = d(0);
            } catch (Exception e) {
                // ignore
            } finally {
                c = b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
    }

    public static void main(String[] args)
    {
        try{
           new Main().test();
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
