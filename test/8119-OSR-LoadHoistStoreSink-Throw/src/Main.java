
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
        int a0 = 0x7;

        try 
        {
            for (int i = 0; i < 10*1000*1000; i++)
            {
                x.value = a0;
                a0/=2;
//                throw new java.lang.ArithmeticException();
            }
        }catch(java.lang.ArithmeticException e){
        }
        return x.value;
    }

    public void test()
    {
        int sum = 0;
        for (int i = 0; i < 10; i++)
        {
            sum = sum + testLoop();
        }
        System.out.println(testLoop());
    }

    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
