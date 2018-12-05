
public class Main
{
    public char[]  value = new char[3];

    public char testLoop(char a0)
    {
        int iterations = 10;
        for (int i = 0; i < iterations; i++)
        {
            a0 = 'b';
            value[0] = a0;
        }

        return value[0];
    }

    public void test()
    {
        System.out.println(testLoop('a'));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
