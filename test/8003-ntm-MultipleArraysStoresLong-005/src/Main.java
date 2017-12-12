
// M: no non temporal move expected because of BoundsCheck
// N: non temporal move is applied because BoundsCheck is eliminated

public class Main {                                                                                                                                                   

    final int iterations = 0x40000;

    class A {
        public long[] arr1 = new long [iterations];
        public long[] arr2 = new long [iterations];
    
        public long[] testLoop(long[] tab) {
        for (int i = 0; i < iterations; i++) {
            tab[i] = (long)(i + 1);
            arr1[i] = (long)(i * 2);
            tab[i] = (long)i * 3;
            arr1[i] = (long)i * 4;
            arr2[i] = (long)i * 5;
        }
        arr1[iterations/2] = tab[10];
        tab[11] = arr1[5] + arr2[5];
        return tab;
    }

    }

    public void test()
    {
        long[] tab1 = new long [iterations];
        A a = new A();
        System.out.println(a.testLoop(a.arr1)[11]);
        System.out.println(a.arr1[11]);
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

