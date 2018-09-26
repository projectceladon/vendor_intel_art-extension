
// negative
// loadhoist_storesink: Set v77 ArraySet [ l143 i145 i62 ] aliases with instruction i62 ArrayGet [ l23 i103 ].
// loadhoist_storesink: There are 0 get/set pairs and 0 sets to sink out of the loop.

public class Main
{
    public static class A
    {
        public static int[] value = new int[3];
    }

    public int testLoop(int a0)
    {
        A x;
        x = new A();
        int[] arr = new int[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = 1 + i + i * i;
            a0 = arr[i];
            x.value[0] = a0;
        }

        return x.value[0];
    }

    public void test()
    {
        System.out.println(testLoop(0x7));
    }
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
