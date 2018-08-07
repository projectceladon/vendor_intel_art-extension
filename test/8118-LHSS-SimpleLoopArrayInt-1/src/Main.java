
public class Main
{
    public int[]  value = new int[3];

    public int testLoop(int a0)
    {
        value[0] = a0;

        for (int i = 0; i < 10; i++)
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
