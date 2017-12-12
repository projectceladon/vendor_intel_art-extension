
//M: No non temporal move expected because BoundsCheck is not eliminated for this loop
// N: non temporal move expected because BoundsCheck is eliminated for this loop
public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    final static int add_gs = 3;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(long[] tab) {
        int i = 1;
        int add_l = 4;
        tab[0] = 0;
        do {
            tab[i] = i*i^(i%2) + add_gs + add_l;
        } while (++i < iterations);

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

