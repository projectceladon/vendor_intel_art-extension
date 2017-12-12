
//1 non-temporal move expected: accessing array in separate loop

public class Main {
    final int iterations = 133000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public long testLoop(long[] tab) {
        long s = 0;
        for (int i = 0; i < iterations; i++) {
            for (int j = 0; j < iterations; j++) {
                tab[j] = j;
            }
            s = s + tab[i]%2;
        }
        return checkSum(tab, iterations);
    }

    public void test()
    {
        long[] tab = new long[iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

