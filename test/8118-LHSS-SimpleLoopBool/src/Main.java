
public class Main
{
    public class A 
    {
        public boolean value;
    }

    public boolean testLoop()
    {
        A x;
        x = new A();
        boolean a0 = true;
        boolean b0 = false;

        for (int i = 0; i < 10; i++)
        {
            a0 = a0 & b0;
            x.value = a0;
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
