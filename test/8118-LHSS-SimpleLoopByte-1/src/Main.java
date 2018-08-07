
public class Main
{
    public class base
    {
        public byte value;
    }

    public byte testLoop()
    {
        base x;
        x = new base();
        byte a0 = 0x77;

        for (byte i = 0; i < 10; i++)
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
