
public class Main
{
    public static class A 
    {
        public static int value;
    }

    public int testLoop(int a0)
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
        System.out.println(testLoop(0x70000007));
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
