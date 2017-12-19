
public class Main
{
    public char[]  value = new char[3];

    public char testLoop(char a0)
    {
        int iterations = 8;
        for (int i = 0; i < iterations; i++)
        {
            a0 = 'b';
            value[0] = a0;
            value[1] = (char)(i*0x11);
        }

        return value[1];
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
