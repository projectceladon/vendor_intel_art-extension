
// negative so far, see CAR-2661

public class Main
{
    public static int field = 0;

    public int testLoop()
    {
        int iterations = 50;
        for (int i = 0; i < iterations; i++)
        {
            int a = field;
            field = a + i;
            int b = field;
            field = b + 2 * i;
        }

        return field;
    }

    public void test()
    {
        System.out.println(testLoop());
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
