
//no non-temporal move expected: load operation:  accessing instance field within loop

public class Main {                                                       


    class C {
        long value = 10L;
    }

    final int iterations = 0x40000;
    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(long[] tab) {
        C c = new C();
        for (int i = 0; i < iterations; i++) {
            c.value += i/2;
            tab[i] = c.value;
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

