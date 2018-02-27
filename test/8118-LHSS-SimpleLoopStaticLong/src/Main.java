
public class Main
{
    public static class A
    {
        public static long value;
    }

    public long testLoop(long a0)
    {
        A x;
        x = new A();
// pass a0, via parameter to isolate LHSS pass from other optimization passes

        for (int i = 0; i < 10; i++)
        {
            a0++;
            x.value = a0;
        }

        return x.value;
    }

    public void test()
    {
        System.out.println(testLoop(0x7000000000000007L));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
