
// 7 non temporal moves expected in test(): testLoop() is used 5 times, inlined. 
// NTM generated 2 times instead of 1 in try and finally blocks

public class Main {
    final int iterations = 133000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    final long testLoop(long[] tab) {
        for (int j = 0; j < iterations; j++) {
            tab[j] = j;
        }
        return checkSum(tab, iterations);
    }


    final void test()
    {
        long[] tab = new long[iterations];

        long res = 0;

        res += testLoop(tab); // Before try-catch.

        try {
            if (testLoop(tab) > 0)
                throw new Exception("..."); // In Try section
        } catch (Exception e) {
            res ^= testLoop(tab); // In catch secton.
        } finally {
            res -= testLoop(tab); // in Finally section.
        }

        res *= testLoop(tab); // After try-catch.

        System.out.println(res);
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}