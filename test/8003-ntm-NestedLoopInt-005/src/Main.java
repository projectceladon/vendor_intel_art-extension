
// 2 non temporal move expected

public class Main {                                                                                                                                                   

    final int iterations = 133000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab1, int[] tab2) {
        for (int k = 0; k < 3; k++) {
            for (int i = 0; i < iterations; i++) {
                tab1[i] = i;
            }
            for (int i = 0; i < iterations; i++) {
                tab2[i] = i;
            }
        }
        return checkSum(tab1, iterations) - checkSum(tab2, iterations);
    }

    public void test()
    {
        int[] tab1 = new int [iterations];
        int[] tab2 = new int [iterations];
        System.out.println(testLoop(tab1, tab2));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

