
public class Main
{
    public static class A
    {
        public static long[]  value = new long[3];
    }

    public long testLoop(long a0)
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
        System.out.println(testLoop((long)0x70000007));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
