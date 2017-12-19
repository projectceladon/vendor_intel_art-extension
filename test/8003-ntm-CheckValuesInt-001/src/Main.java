
// 1 non temporal move expected

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab1, int[] tab2) {
        int s = 0; 
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
        int[] tab1 = new int [0x40000];
        int[] tab2 = new int [0x40000];
        System.out.println(testLoop(tab1, tab2));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

