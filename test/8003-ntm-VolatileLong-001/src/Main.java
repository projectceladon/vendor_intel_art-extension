
// 2 non-temporal moves expected. Assign volatile variable to tab[i]

public class Main {
    final int iterations = 133000*200;
    private volatile long jack = 0;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public boolean testLoop(long[] tab) {
        new Thread(new Runnable() {
            public void run() {
                for(int i = 0; i < iterations; ++i) {
                    jack = 1;
                }
            }
        }).start();

        long count = 0;
        for(int i = 0; i < iterations; ++i) {
            long k = jack;
            tab[i] = k;

            if (k == 1) {
                count++;
                jack = 0;
            }
        }

        return checkSum(tab, iterations) == count;
    }

    public void test()
    {
        long[] tab = new long[iterations];

        if (testLoop(tab) == true)
            System.out.println("ok");
        else
            System.out.println("not ok");
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}

