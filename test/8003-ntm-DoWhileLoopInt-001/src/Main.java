
// M: No non temporal move expected because BoundsCheck is not eliminated for this loop
// N: non temporal move expected because BoundsCheck is  eliminated for this loop

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    final static int add_gs = 3;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab) {
        int i = 0;
        int add_l = 4;
        do {
            tab[i] = i + add_gs + add_l;
            i++;
        } while (i < iterations);

        return checkSum(tab, iterations);
    }

    public void test()
    {
        int[] tab = new int [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

