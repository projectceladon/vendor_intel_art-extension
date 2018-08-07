
public class Main
{
    public class A 
    {
        public int[] value = new int[3];
    }

    public int testLoop()
    {
        A x;
        x = new A();

        int []a0 = {0x7, 0x77, 0x707};
        int []b0 = {0x7007, 0x70007, 0x700007};

        for (int i = 0; i < 10; i++)
        {
            b0[0]++;
			
            if (i%2 == 0)
            {
                a0 = b0;
            }

            x.value = a0;
        }

        return x.value[0];
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
