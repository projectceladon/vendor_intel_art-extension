
public class Main
{
    public int[] value = new int [3];

    public int testLoop()
    {
        int a0 = 0x7007;

        try { 
        for (int j = 0; j < 10; j++)
        {
            a0++;
            value[0] = a0;
        }

        throw new Exception();}
        catch (Exception e){ a0++; }
        finally { a0-=1; }

        return value[0];
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
