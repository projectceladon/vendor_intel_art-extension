
public class Main
{
    public class A 
    {
        public int value;
    }

    public int testLoop()
    {
        A x;
        x = new A();

        int a0 = 0x7;
        int b0 = 0x77;

        for (int i = 0; i < 10; i++)
        {
            if (i%2 == 0)
            {
                a0 = b0;
            }
            x.value = a0;
            a0++;
        }
        return x.value;
    }

    public void test()
    {
        System.out.println(testLoop());
    }

    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
