
public class Main
{
    public static class A
    {
        public static byte[]  value = new byte[3];
    }

    public byte testLoop(byte a0)
    {
        int iterations = 100;
        for (int i = 0; i < iterations; i++)
        {
            a0++;
            A.value[0] = a0;
        }

        return A.value[0];
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
