
public class Main
{
    public boolean[] value = new boolean[3];

    public boolean testLoop(boolean a0)
    {
        int iterations = 100;
        value[0] = a0;
        for (int i = 0; i < iterations; i++)
        {
            value[0] = value[0] || false;
        }

        return value[0];
    }

    public void test()
    {
        System.out.println(testLoop(true));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
