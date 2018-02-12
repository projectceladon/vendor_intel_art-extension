
public class Main
{
    public class A 
    {
        public int value = 1000;
    }
    public int field = 0;

    //OSR + LHSS after try-catch block
    public int testLoop(int n)
    {
        A x;
        x = new A();
        A y = new A();
        int a0 = 0x70000007;
        int j = 0;


        try {
                x.value += n/(n - 10);
        } catch (Exception e) {
            System.out.println("Exception caught");
            x.value++;
        } finally {
            j = 0;
            y.value--;
        }
        do  {     
            x.value++;
        } while ( j++ < 30*100*1000);


        return x.value + y.value;
    }


    public static void main(String[] args)
    {
        System.out.println(new Main().testLoop(10));        
    }    
}
