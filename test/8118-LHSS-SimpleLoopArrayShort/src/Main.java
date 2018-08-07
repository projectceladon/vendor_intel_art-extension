
public class Main
{
    public short[]  value = new short[3];
	   
    public short testLoop(short a0)
    {
        int iterations = 100;
        for (int i = 0; i < iterations; i++)
        {
            a0++;
            value[0] = a0;
        }

        return value[0];
    }

    public void test()
    {
        System.out.println(testLoop((short)0x7007));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
