// positive
// loadhoist_storesink: Gets and sets for method "int OptimizationTests.LoopInvariantSinking.RangeCheckSetter_5.Main.testLoop(int)"
// loadhoist_storesink: There are 0 get/set pairs and 1 sets to sink out of the loop.
// loadhoist_storesink: Set "v59 InstanceFieldSet [ l7 i53 ]" has been successfully sunk out of the loop #3 in block #6 successfully!


public class Main
{
    public int value;

    public int testLoop(int a0)
    {
        int[] arr = new int[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = 1 + i + i * i;
            value = arr[i];
        }

        return value;
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
