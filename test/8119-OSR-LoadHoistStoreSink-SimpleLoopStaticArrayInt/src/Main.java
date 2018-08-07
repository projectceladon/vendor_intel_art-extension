
public class Main
{
    public static class A
    {
	public static int[]  value = new int[3];
    }

    public int testLoop(int a0)
    {
        int iterations = 100;
        for (int i = 0; i < iterations*1000*100; i++)
        {
            A.value[0] = a0;
            a0++;
        }

        return A.value[0];
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
