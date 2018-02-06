import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Main {
    /*Simple test*/
    private int $noinline$testSimpleIfElseMultipleVars(int p) {
        int[] a = new int[100];
        int res = a[p];
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            double y = 6;
            if (res * 70 < 10) {
                x += Math.cos(res);
            } else {
                y -= Math.sin(res);
            }
            res += i;
        }
        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testSimpleIfElseMultipleVars(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }


}

