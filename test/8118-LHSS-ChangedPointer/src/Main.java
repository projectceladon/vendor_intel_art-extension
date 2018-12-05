
public class Main
{
    public class A 
    {
        public int value;
    }

    public class A_1 extends A
    {
        public int value;
    }

    public int testLoop()
    {
        A x;
        x = new A();
        A_1 y;
        y = new A_1();

        int a0 = 0x7;

        for (int i = 0; i < 10; i++)
        {
            if (i % 2 == 0)
                x = y;
            x.value = a0;
        }

        return x.value + y.value;
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
