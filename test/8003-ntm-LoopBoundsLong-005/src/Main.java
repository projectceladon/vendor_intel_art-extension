
// No non temporal move expected: unknown number of iterations/non-constant number of iterations

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(long[] tab) {
        int i = 0;
        do {
            tab[i] = i*i^(i%2);
            ++i;
        } while (checkSum(tab, i-1) < 2621440 + 1);

        return checkSum(tab, i);
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

