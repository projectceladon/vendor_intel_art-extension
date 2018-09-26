
public class Main
{
    public long[]  value = new long[3];

    public long testLoop(long a0)
    {
        int iterations = 100;
        try { throw new Exception();}
        catch (Exception e){ value[0]++; }
        finally {
        for (int i = 0; i < iterations; i++)
        {
            a0++;
            value[0] = a0;
        }

        value[0]-=1; }


        return value[0];
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
