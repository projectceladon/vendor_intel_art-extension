
// negative, memory aliasing set:
// loadhoist_storesink: Set v68 ArraySet [ l113 i115 i57 ] aliases with instruction i57 ArrayGet [ l14 i94 ]

public class Main
{
    public int[] value = new int [3];

    public int testLoop()
    {
        int[] arr = new int[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = 1 + i + i * i;
            value[0] = arr[i];
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
