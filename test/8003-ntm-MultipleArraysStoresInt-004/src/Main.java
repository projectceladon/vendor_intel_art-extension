
// M: no non temporal move expected because of BoundsCheck
// N: non temporal move is applied because BoundsCheck is eliminated

public class Main {                                                                                                                                                   

    final int iterations = 0x40000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab1, int[] tab2) {
        for (int i = 0; i < iterations; i++) {
            tab1[i] = i;
            tab2[i] = i;
        }
        return checkSum(tab1, iterations) - checkSum(tab2, iterations);
    }

    public void test()
    {
        int[] tab1 = new int [iterations];
        System.out.println(testLoop(tab1, tab1));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

