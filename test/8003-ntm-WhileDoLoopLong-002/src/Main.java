
// 1 non temporal move expected: BoundsCheck is eliminated for this loop. see CAR-4140

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;

    public long $noinline$checkSum(long[] tab, int start, int end) {
        long s = 0;
        for (int i = 0; i < end ; i++) {
            s = s + tab[i];
        }
        return s;
    }

    public long $noinline$testLoop(long[] tab) {
        
        int i = 100;
        while (++i < iterations-5) {
            tab[i] = (i>>1)/100;
        }
        return $noinline$checkSum(tab, 101, iterations-4);
    }

    public void $noinline$test()
    {
        long[] tab = new long [iterations];
        System.out.println($noinline$testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().$noinline$test();
    }

}  

