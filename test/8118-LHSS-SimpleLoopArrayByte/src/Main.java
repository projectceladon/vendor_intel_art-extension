
// remove_loop_suspend_checks: Loop is not countable
// loadhoist_storesink: Set v41 ArraySet [ l83 i85 b26 ] is contained in a loop with suspend point, and its input is neither loop header phi nor invariant
// loadhoist_storesink: There are 0 get/set pairs and 0 sets to sink out of the loop

public class Main
{
    static int iterations = 100000;
    public byte[]  value = new byte[3];

    public byte testLoop(byte a0)
    {

        for (int i = 0; i < iterations; i++)
        {
            a0++;
            value[0] = a0;
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
