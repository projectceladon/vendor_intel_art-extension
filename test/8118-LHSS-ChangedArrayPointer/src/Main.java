
public class Main
{
    public class A 
    {
        public int []value = new int[3];
    }

    public class A_1 extends A
    {
       public int []value = new int[3];
    }

    public int testLoop()
    {
        A x;
        x = new A();
        A_1 y;
        y = new A_1();

        int a0[] = {0x7, 0x77, 0x707};
        int b = 0x7007;	

        for (int i = 0; i < 10; i++)
        {
            b ++;
            if (i%2 == 0)
            {
                x = y; 
            }
            x.value = a0;
        }
        return x.value[0] + y.value[0];
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
