
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

        for (int i = 0; i < 10; i++)
        {
            a0++;
            if (i < 500)
                x.value = a0;
            else
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
