
public class Main
{
    public class A 
    {
        public int value = 1000;
    }
    public int field = 0;


    public void inlineMe(A x) {
        int j = 0;
        // set/get pair will be hoisted/sunk out of the loop
        do  {     
            x.value++;
//            x.value /= 2;
        } while ( j++ < 30*1000);


    }

    //after inlining we will have OSR + LHSS in catch block
    public int testLoop(int n)
    {
        A x;
        x = new A();
        int a0 = 0x70000007;
        int j = 0;


        try {
                x.value += n/(n - 10);
        } catch (Exception e) {
            System.out.println("Exception caught");
            do  {
                x.value++;
                inlineMe(x);
            } while (j++ < 30*10*100);
        }

        return x.value;
    }


    public static void main(String[] args)
    {
        System.out.println(new Main().testLoop(10));        
    }    
}
