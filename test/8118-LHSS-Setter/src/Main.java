
public class Main
{
    public class A 
    {
        public int value;
        public Object obj;
    }

    public Object testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x7;
        int b0 = 0x77;

        for (int i = 0; i < 10; i++)
        {
            a0 = a0 + i * i;
            x.obj = null;
            b0 ++;
        }
        return a0 + b0;
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
