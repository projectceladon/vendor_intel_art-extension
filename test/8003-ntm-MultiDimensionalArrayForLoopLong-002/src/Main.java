

// No non temporal move expected because accessing (aget) outer array in the loop 

public class Main {                                                              
    final int iterations = 133000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }


    public long testLoop(long[][] tab) {
        for (int i = 0; i < 10; i++) {
            for (int j=0; j < iterations; j++) {
                tab[i][j] = j;
            }
        }
        return checkSum(tab[0], iterations);
    }

    public void test()
    {
        long[][] tab = new long[10][iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

