
// 1 non temporal move expected
public class Main {                                                                                                                                                   

    final int iterations = 133000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public static int foo(int i) {
        return i/2+1;
    }


    public long testLoop(int[] tab) {
        for (int i = 0; i < foo(10); i++) {
            tab[i] = i;
            for (int j=2; j>0; j--) {
                int l = 0;
                do {
                    int k=0;
                    while (k < iterations) {
                        tab[k] = k;
                        k++;
                    }
                    l=l+2;
                } while (l<10);

            }
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

