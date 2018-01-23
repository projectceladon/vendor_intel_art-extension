import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Main {
    /*Simple test*/
    private int $noinline$testSimpleIfElse(int p) {
        int[] a = new int[100];
        int res = a[p];
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            } else {
                x -= Math.cos(res);
            }
            res += i;
        }
        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testSimpleIfElse(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }

}
