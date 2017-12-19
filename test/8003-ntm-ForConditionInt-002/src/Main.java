
// N: 2 non-temporal move expected: 1 if condition in the loop
// M: no non-temporal move expected

public class Main {
    final int iterations = 133000;

    public int $noinline$checkSum(int[] tab, int n) {
        int s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public int $noinline$testLoop(int[] tab) {
        for (int j = 0; j < iterations; j++) {
            tab[j] = j;
            if (j > 4)
                tab[j]++;
        }
        return $noinline$checkSum(tab, iterations);
    }

    public void $noinline$test()
    {
        int[] tab = new int[iterations];
        System.out.println($noinline$testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().$noinline$test();
    }

}

