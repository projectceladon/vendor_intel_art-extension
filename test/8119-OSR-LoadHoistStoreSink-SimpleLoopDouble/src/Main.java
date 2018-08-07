
public class Main
{
    public class A 
    {
        public double value;
    }

    public double testLoop()
    {
        A x;
        x = new A();
        double a0 = 1.7777777;

        for (int i = 0; i < 10*1000*1000; i++)
        {
            x.value = a0;
            a0++;
        }

        return x.value;
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
