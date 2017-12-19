
// No non temporal move expected: optimization is only applied to int and long type arrays

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    
    public long checkSum(byte[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(byte[] tab) {
        
        for (int i = 0; i < iterations; i++) {
            tab[i] = 10;
        }
        return checkSum(tab, iterations);
    }

    public void test()
    {
        byte[] tab = new byte [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

