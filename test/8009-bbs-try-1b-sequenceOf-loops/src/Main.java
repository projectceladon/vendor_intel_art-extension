import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class Main {
    /*Simple test*/
    private int $noinline$testSequenceOfLoops(int p) {
        int[] a = new int[100];
        a[p] = 170;
        int res = a[p];
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            }
            else if (res * 70 < 20) {
                x -= Math.cos(res);
            }
            else if (res * 70 < 30) {
                x += Math.sin(res);
            }
            else {
                x -= Math.sin(res);
            }
            res += i;
        }
        for (int i = 0; i < 2000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            }
            else if (res * 70 < 20) {
                x -= Math.cos(res);
            }
            else if (res * 70 < 30) {
                x += Math.sin(res);
            }
            else {
                x -= Math.sin(res);
            }
            res += i;
        }

        try { throw new Exception();}
        catch (Exception e){ res++; }
        finally { res-=2; }


        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testSequenceOfLoops(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }


}
