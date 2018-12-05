
public class Main
{
    public class A 
    {
        public short value;
    }

    public short testLoop()
    {
        A x;
        x = new A();
        short a0 = 0x7007;

        for (int i = 0; i < 10; i++)
        {
            a0++;
            x.value = a0;
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
