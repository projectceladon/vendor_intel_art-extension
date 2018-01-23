import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class Main {
    /*Simple test*/
    private int $noinline$testNestedIfsD2(int p) {
        int[] a = new int[100];
        a[p] = 16;
        int res = a[p];
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * res >35534543) {
                if (res * 70 < 20) {
                    x += Math.sin(res);
                }
                else {
                    x += Math.cos(res);
                }
            }
            res += i;
        }
        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testNestedIfsD2(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }


}

