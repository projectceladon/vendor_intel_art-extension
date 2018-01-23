import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class Main {
    /*Simple test*/
    private int $noinline$testSequenceOfSimpleIfsOneVar(int p) {
        int[] a = new int[100];
        a[p] = 190;
        int res = a[p];
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            }
            else {
                x -= Math.cos(res);
            }
            if (res * res < 900) {
                x *= Math.cos(res);
            }
            else {
                x /= Math.sin(res);
            }
            if (res * res * res < 3000) {
                x += Math.sin(res);
            }
            else {
                x -= Math.sin(res);
            }
            res += i;
        }
        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testSequenceOfSimpleIfsOneVar(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }
}

