
// N: 2 non-temporal move expected: 1 if condition in the loop
// M: no non-temporal move expected

public class Main {
    final int iterations = 133000;

    public long $noinline$checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    // Formally, NTM should not apply here, because the loop contains the array get. In this particular case, the compiler can prove that tab[j] is actually equal to j (because it was set just above).
    public long $noinline$testLoop(long[] tab) {
        for (int j = 0; j < iterations; j++) {
            tab[j] = j;
            if (j > 4)
                tab[j] = tab[j] + 1; // Array get!!!

        }
        return $noinline$checkSum(tab, iterations);
    }

    public void $noinline$test()
    {
        long[] tabLong = new long[iterations];
        System.out.println($noinline$testLoop(tabLong));
    }

    public static void main(String[] args)
    {
        new Main().$noinline$test();
    }

}

