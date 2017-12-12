
// No non temporal move expected: optimization is only applied to int and long type arrays

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    
    public double checkSum(double[] tab, int n) {
        double s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public double testLoop(double[] tab) {
       
        double d = 0.5d; 
        for (int i = 0; i < iterations; i++) {
            tab[i] = i + d;
        }
        return checkSum(tab, iterations);
    }

    public void test()
    {
        double[] tab = new double [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

