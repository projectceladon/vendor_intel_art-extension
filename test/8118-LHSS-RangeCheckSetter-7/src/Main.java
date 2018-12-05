
public class Main
{
    public class A
    {
        public int[] value = new int[3];
    }

    public int testLoop()
    {
        A x;
        x = new A();
        int[] arr = new int[100];
        int a0 = 1; 
        int b0 = 0;
        for (int i = 0; i < 100; i++) {
            arr[i] = a0 + i + i * i;
            b0 = arr[i];
            x.value[0] = b0;
        }

        return x.value[0];
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
