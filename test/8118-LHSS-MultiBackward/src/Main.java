
public class Main
{
    public class A 
    {
        public int value;
    }

    public long testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x7;

        int i = 0;
        while(i < 10000000)
        {
            a0++;
            x.value = a0;
            
            if (i%2 == 0)
            {
                i++;
                continue;
            }
            i = i+2;
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
