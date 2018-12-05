
public class Main
{
    public int changel(int i)
    {
        for (int j = 0; j < 10; j++)
            i++;
        i = i - 7;
        i = i * i;
        return i - 25;
    }
    
    public class A
    {
        public int value;
    }

    public int testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x7;
        
        for (int i = 0; i < 10; i++)
        {
            a0 += changel(a0);
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
