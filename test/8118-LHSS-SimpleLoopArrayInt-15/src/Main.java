
public class Main
{
    static final int iterations = 100000;
    public int[]  value = new int[3];

    public int testLoop(int a0)
    {
        value[0] = a0;

        for (int i = 0; i < iterations; i++)
        {
            value[0]++;
        }

        return value[0];
    }

    public void test()
    {
        System.out.println(testLoop((int)0x7));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
