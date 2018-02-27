
public class Main
{
    public class A
    {
        public int [][]value = new int [1][2];
    }

    public int testLoop()
    {
        A x;
        x = new A();
        int [][]a0 = {{0x7}, {0x77}}; 

        for (int i = 0; i < 10; i++)
        {
            a0[0][0] ++;
            x.value = new int[1][2];
            x.value = a0;
        }

        return x.value[0][0];
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
