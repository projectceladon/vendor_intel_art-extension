
// LHSS occurs despite the fact that suspend point has not been removed due to casting:
// remove_loop_suspend_checks: Loop is not countable
// this is the case when fake suspend is created

public class Main
{
    public static class A
    {
        public static byte[]  value = new byte[3];
    }

    public byte testLoop(byte a0)
    {
        byte iterations = (byte)127;
        A.value[0] = a0;

        for (byte i = 0; i < iterations; i++)
        {
            A.value[0]++;
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
