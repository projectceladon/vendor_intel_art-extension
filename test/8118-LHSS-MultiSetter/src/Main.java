
public class Main
{
    public class A 
    {
        public int val0;
        public int val1;
    }

    public int testLoop()
    {
        A x;
        x = new A();

        int a0 = 0x7;
        int a1 = 0x77;

        for (int i = 0; i < 10; i++)
        {
            a0 ++;
            x.val0 = a0;
            a1 ++;
            x.val1 = a1;
        }

        return x.val0 + x.val1;
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
