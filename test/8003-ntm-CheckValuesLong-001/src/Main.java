
// 1 non temporal move expected

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long checkSum(long[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(long[] tab1, long[] tab2) {
        long s = 0;        
        //optimization is applied
        for (int i = 0; i < iterations; i++) {
            tab1[i] = i;
        }
        //optimization is not applied because it's not an inner loop
        for (int i = 0; i < iterations; i++) {
            tab2[i] = i;
            for (int j = 0; j<1; j++) {
                s += j%3;
            }
        }

        for (int i = 0; i < iterations; i++) {
            if (tab1[i] != tab2[i]) {
                return -1;
            }
        }
        

        return checkSum(tab1, iterations) + s;
    }

    public void test()
    {
        long[] tab1 = new long [0x40000];
        long[] tab2 = new long [0x40000];
        System.out.println(testLoop(tab1, tab2));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

