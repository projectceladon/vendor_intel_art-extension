
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

    // After inlining we will have outer loop handled by OSR and inner loop will have LHSS applied 
    // According to QA doc: * OSR injections will not be done to Try block. - but in this case OSR is applied w/o silver optimizations to outer loop only
    public int testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x70000007;
        int j = 0;


        try {
            do  {
                inlineMe(x);
            } while (j++ < 30*10*100);
        } catch (Exception e) {
            System.out.println(e);
        }

        return x.value;
    }


    public static void main(String[] args)
    {
        System.out.println(new Main().testLoop());        
    }    
}
