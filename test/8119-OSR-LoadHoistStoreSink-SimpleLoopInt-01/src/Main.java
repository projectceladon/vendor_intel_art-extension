
public class Main
{
    public class A 
    {
        public int value;
    }
    public int field = 0;
    public int testLoop()
    {
        A x;
        x = new A();
        int a0 = 0x70000007;

        // set/get pair will be hoisted/sunk out of the loop
        for (int j =0; j < 100*1000*1000; j++) {     
            x.value++;
        }

        return x.value;
    }


    public static void main(String[] args)
    {
        System.out.println(new Main().testLoop());        
    }    
}
