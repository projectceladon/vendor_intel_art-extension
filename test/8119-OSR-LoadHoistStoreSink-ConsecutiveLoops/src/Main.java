
public class Main
{
    public class A 
    {
        public int value;
    }

    public int testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x70000007;

        for (int i = 0; i < 10*1000*1000; i++)
        {
            x.value = a0;
            a0++;
        }
        a0++;
        for (int i = 0; i < 10*1000*1000; i++)
        {
            x.value = a0;
            a0 += 2;
        }
        for (int i = 0; i < 10*1000*1000; i++)
        {
            x.value = a0;
            a0 += 3;
        }

        a0--;

        for (int i = 0; i < 10*1000*1000; i++)
        {
            x.value = a0;
            a0 -=1;
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
