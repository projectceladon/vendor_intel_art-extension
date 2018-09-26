
public class Main
{
    static int iterations = 10;
    public byte[]  value = new byte[3];

    public byte testLoop(byte a0)
    {
        value[0] = a0;

        for (int i = 0; i < 10; i++)
        {
            value[0]++;
        }

        return value[0];
    }

    public void test()
    {
        System.out.println(testLoop((byte)0x7));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
