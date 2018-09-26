
// memory aliasing (see also SimpleLoopStaticArrayByte_3

public class Main
{
    public static class A
    {
        public static byte[]  value = new byte[3];
    }

    public byte testLoop(byte a0)
    {
        byte[] iterations = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
        A.value[0] = a0;

        for (byte i : iterations)
        {
            A.value[0] = (byte)(A.value[0] + i);
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
