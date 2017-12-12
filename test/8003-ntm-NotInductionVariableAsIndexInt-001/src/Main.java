
// No non temporal move expected, breaking limitation (array access index must be induction variable)

public class Main {                                                                                                                                                   

    final int iterations = 0x40000;

    public long checkSum(int[] tab, int start, int end) {
        long s = 0;
        for (int i = start; i < end ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab) {
        for (int i = 0; i < iterations-1; i++) {
            tab[i+1] = i;
        }
        return checkSum(tab, 1, iterations);
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

