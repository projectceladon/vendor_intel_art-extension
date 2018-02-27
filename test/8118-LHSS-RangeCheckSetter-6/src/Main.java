
public class Main
{
    public class A
    {
        public int[] value = new int[3];
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
