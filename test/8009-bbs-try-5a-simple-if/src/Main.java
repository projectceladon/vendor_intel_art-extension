import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class Main {
    /*try_Simple test*/
    private int $noinline$testtry_SimpleIf(int p) {
        int[] a = new int[100];
        a[p] = 17;
        int res = a[p];
        try { throw new Exception(); }
        catch (Exception e){ res++; }
        finally { res-=2; }
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            }
            res += i;
        }
        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testtry_SimpleIf(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }
}


