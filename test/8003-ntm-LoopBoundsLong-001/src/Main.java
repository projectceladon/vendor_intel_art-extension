

// No non temporal move because non-constant bound

public class Main {                                                                                                                                                   

    final int iterations = 0x40000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public long testLoop(long[] tab, int n) {
        if (n == iterations) {
            for (int i = 0; (i < n) ; i++) {
                tab[i] = i;
            }
        }
        return checkSum(tab, n);
    }

    public void test()
    {
        long[] tab = new long [iterations];
        System.out.println(testLoop(tab, iterations));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

