
public class Main
{
    public byte[]  value = new byte[3];

    public byte testLoop(byte a0)
    {
        int iterations = 127;
        value[0] = a0;

        for (byte i = 0; i < iterations; i++)
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
