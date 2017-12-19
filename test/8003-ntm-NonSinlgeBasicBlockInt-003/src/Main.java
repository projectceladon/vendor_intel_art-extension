
// No non temporal move expected: breaking single basic block limitation

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab, int n) {
        
        for (int i = 0; i < iterations; i++) {
            switch (n) {
                case 0: tab[i] = 0;
                        break;
                case 1: tab[i] = 1;
                        break;
                default:
                        tab[i] = i;
                        break;
            }
        }
        
        return checkSum(tab, iterations);
    }

    public void test()
    {
        int[] tab = new int [iterations];
        System.out.println(testLoop(tab,0));
        System.out.println(testLoop(tab,1));
        System.out.println(testLoop(tab,2));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

