
public class Main
{
    public static class A 
    {
        public static int value;
    }

    public int testLoop(int a0, int b0)
    {
        A x;
        x = new A();
        x.value = 0x0;

// pass a0, b0 via parameters to isolate LHSS pass from other optimization passes
//        int a0 = 0x7;
//        int b0 = 0x77;

        for (int i = 0; i < 10; i++)
        {
            if (i%2 == 0)
            {
                a0 = b0;
            }
            a0++;
            x.value = a0;
        }

        return x.value;
    }

    public void test()
    {
        System.out.println(testLoop(0x7, 0x77));
    }

    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
