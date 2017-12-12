
// 2 non-temporal move expected: 1 if condition in the loop

public class Main {
    final int iterations = 133000;

    public long $noinline$checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long $noinline$testLoop(long[] tab) {
        for (int j = 0; j < iterations; j++) {
            tab[j] = j;
            if (j > 4)
                tab[j] = j + 1;
        }
        return $noinline$checkSum(tab, iterations);
    }

    public void $noinline$test()
    {
        long[] tab = new long[iterations];
        System.out.println($noinline$testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().$noinline$test();
    }

}

