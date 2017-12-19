

// 1 non temporal move expected

public class Main {                                                                                                                                                   
    final int iterations = 131075; //Min number of iterations for the optimization to be applied for sure: the threshold is 131072, but 1-2 (maybe even more in future) iterations can be removed by peeling

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(long[] tab) {
        for (int i = 0; i < iterations ; i++) {
            tab[i] = i;
        }

        return checkSum(tab, iterations);
    }

    public void test()
    {
        long[] tab = new long [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

