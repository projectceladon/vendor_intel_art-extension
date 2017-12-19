
//no non-temporal move expected: load operation:  accessing static field within loop

public class Main {                                                       

    static int sti = 1;

    static class C {
        static byte value = 0;
    }

    final static int iterations = 0x40000;

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
            c.value += i/3;
            tab[i] = c.value;
        }
        for (int i = 0; i < iterations; i++) {
            sti += i/2;
            tab[i] = sti;
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

