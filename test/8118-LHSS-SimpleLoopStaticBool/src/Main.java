
public class Main
{
    public static class A 
    {
        public static boolean value;
    }

    public boolean testLoop(boolean a0, boolean b0)
    {
        A x;
        x = new A();
        
// pass a0, b0 via parameters to isolate LHSS pass from other optimizations

        for (int i = 0; i < 10; i++)
        {
			a0 = a0 & b0;
			x.value = a0;
		}

        return x.value;
    }

    public void test()
    {
        System.out.println(testLoop(true, false));
    }

    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
