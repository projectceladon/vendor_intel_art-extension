import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class Main {
    /*Simple test*/
    private int $noinline$testSequenceOfNestedLoops(int p) {
        int[] a = new int[100];
        a[p] = 170;
        int res = a[p];
        try { throw new Exception();}
        catch (Exception e){ res++; }
        finally {
        for (int i = 0; i < 1000000; i++) {
            double x = 0;
            if (res * 70 < 10) {
                x += Math.cos(res);
            }
            else if (res * 70 < 20) {
                for (int j = 0; j < 200; j++) {
                    double y = 0;
                    if (res * 70 < 30) {
                        y += Math.cos(res);
                    }
                    else if (res * 70 < 40) {
                        y -= Math.cos(res);
                    }
                    else if (res * 70 < 50) {
                        y += Math.sin(res);
                    }
                    else {
                        y -= Math.sin(res);
                    }
                    res += j;
                }
            }
            else if (res * 70 < 60) {
                x += Math.sin(res);
            }
            else {
                x -= Math.cos(res);
            }
            res += i;
        }
        res-=2;
        }

        return res;
    }

    public void RunTests()
    {
        System.out.println($noinline$testSequenceOfNestedLoops(1));
    }

    public static void main(String[] args)
    {
        new Main().RunTests();
    }


}
