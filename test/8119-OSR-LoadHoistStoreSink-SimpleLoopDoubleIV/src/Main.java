
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

        for (double i = 0.2; i < 10*1000*1000; i += 0.99)
        {
            x.value = a0;
            a0 += i;
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
