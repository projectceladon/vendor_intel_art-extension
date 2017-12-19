
// TBD: 1 non temporal move expected: this simple static function has been inlined

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public static int[] A = new int [] {0, 1, 2};

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public static int foo(int i) {
        return i;
    }


    public long testLoop(int[] tab) {
        
        for (int i = 0; i < iterations; i++) {
            tab[i] = foo(i);
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

