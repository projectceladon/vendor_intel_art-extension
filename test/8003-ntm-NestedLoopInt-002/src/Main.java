
//No non temporal moves expected

public class Main {                                                                                                                                                   

    final int iterations = 133000;

    public long checkSum(int[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long testLoop(int[] tab) {
        int s = 0;
        for (int i = 0; i < iterations; i++) {
            for (int j = 0; j < 1; j++) {
                s += j%3;
            }
            tab[i] = i;
        }
        return checkSum(tab, iterations) + s;
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

