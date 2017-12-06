
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterationsLessThanThreshold = 50;

    final double testLoopAddLess(double n) {
        double testVar = 1.0d;
        double additionalVar = 0;
        for (int i = -10; i < iterationsLessThanThreshold; i++) {
              testVar += 3.5d;
              additionalVar += (i*2)/5 + n;
        }
        testVar += additionalVar;
        return testVar;
    }

    final double $noinline$testTryCatch(double n) {
        double res = 0;
        res += testLoopAddLess(n);
        try {
            if (testLoopAddLess(n) > 0) {
                throw new Exception("My exception"); 
            }
        } catch (Exception e) {
            res /= testLoopAddLess(n);
        } finally {
            res -= testLoopAddLess(n);
        }
        res *= testLoopAddLess(n);
        return res;
    }

    public static void main(String[] args) {
         System.out.println(Double.doubleToLongBits(new Main().$noinline$testTryCatch(1.2d)));
    }
}  

