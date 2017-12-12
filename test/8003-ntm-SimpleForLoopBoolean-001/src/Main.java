
// No non temporal move expected: optimization is only applied to int and long type arrays

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    
    public long checkSum(boolean[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + (tab[i] ? 1 : 0);
        }
        return s;
    }

    public long testLoop(boolean[] tab) {
        for (int i = 0; i < iterations; i++) {
            tab[i] = (i%2 != 0);
        }
        return checkSum(tab, iterations);
    }

    public void test()
    {
        boolean[] tab = new boolean [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

