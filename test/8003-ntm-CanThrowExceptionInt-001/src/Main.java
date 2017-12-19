
// No non temporal move expected: violating "loop can't throw exception" condition

public class Main {
    final int iterations = 0x40000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab) {
        try {
            for (int i = 0; i < iterations; i++) {
                tab[i] = i/(i-1);
            }
        } catch (ArithmeticException ae) {
            System.out.println(ae);
        }


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


