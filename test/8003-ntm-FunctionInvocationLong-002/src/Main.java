
// non temporal move expected: function is inlined

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public static int foo(int i) {
        if (i < 100) {
            return i;
        } else {
            return 100;
        }
    }


    public long testLoop(long[] tab) {
        
        for (int i = 0; i < iterations; i++) {
            tab[i] = foo(i);
        }
        
        return checkSum(tab, iterations);
    }

    public void test()
    {
        long[] tab = new long [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

