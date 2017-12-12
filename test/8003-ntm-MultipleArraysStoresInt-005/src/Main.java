
// M: no non temporal move expected because of BoundsCheck
// N: non temporal move is applied because BoundsCheck is eliminated

public class Main {                                                                                                                                                   


    class A {
        public int[] arr1 = new int [iterations];
        public int[] arr2 = new int [iterations];
    
        public int[] testLoop(int[] tab) {
        for (int i = 0; i < iterations; i++) {
            tab[i] = i + 1;
            arr1[i] = i * 2;
            tab[i] = i * 3;
            arr1[i] = i * 4;
            arr2[i] = i * 5;
        }
        arr1[iterations/2] = tab[10];
        tab[11] = arr1[5] + arr2[5];
        return tab;
    }

    }


    final int iterations = 0x40000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public void test()
    {
        int[] tab1 = new int [iterations];
        A a = new A();
        System.out.println(a.testLoop(a.arr1)[11]);
        System.out.println(a.arr1[11]);
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

