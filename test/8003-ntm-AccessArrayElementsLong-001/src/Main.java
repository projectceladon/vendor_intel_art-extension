
//no non-temporal move expected: accessing array within loop

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long testLoop(long[] tab) {
        long s = 0;
        for (int i = 0; i < iterations; i++) {
            tab[i] = i;
            s = s + tab[i/2]%2;
        }
        return s;
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

