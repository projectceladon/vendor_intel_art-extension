// negative
// loadhoist_storesink: Set v88 ArraySet [ l140 i142 i77 ] aliases with instruction i77 ArrayGet [ l17 i121 ]

public class Main
{
	public int[] value = new int [3];

    public int testLoop()
    {
        int a0 = 0x7007;
        int[] arr = new int[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = 1 + i + i * i;
        }

        for (int j = 0; j <100; j++) {
            a0++;
	    value[0] = arr[j];
        }


        return value[0];
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
