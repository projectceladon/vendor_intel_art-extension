
// No non temporal move expected: optimization is only applied to int and long type arrays

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;
    
    public float checkSum(float[] tab, int n) {
        float s = 0;
        for (int i = 0; i < n ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public float testLoop(float[] tab) {
       
        float f = 0.5f; 
        for (int i = 0; i < iterations; i++) {
            tab[i] = i + f;
        }
        return checkSum(tab, iterations);
    }

    public void test()
    {
        float[] tab = new float [iterations];
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

