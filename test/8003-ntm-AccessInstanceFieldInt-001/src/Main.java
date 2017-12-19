
//no non-temporal move expected: load operation:  accessing instance field within loop

public class Main {                                                       


    final int iterations = 0x40000;

    class C {
        int value = 0;
        int value1 = 1;
    }

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab) {
        C c = new C();
        for (int i = 0; i < iterations; i++) {
            c.value = c.value + i/2;
            tab[i] =  c.value;

        }
        System.out.println(c.value);
        return checkSum(tab, iterations);
    }

    public void test()
    {
        int[] tab = new int[iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

