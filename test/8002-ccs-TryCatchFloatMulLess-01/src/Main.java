
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterationsLessThanThreshold = 50;

    final float testLoopMulLess(float n) {
        float testVar = 1.0f;
        float additionalVar = 0;
        for (int i = -10; i < iterationsLessThanThreshold; i++) {
              testVar *= 1.1f;
              additionalVar += (i*2)/5 + n;
        }
        testVar += additionalVar;
        return testVar;
    }

    final float $noinline$testTryCatch(float n) {
        float res = 0;
        res += testLoopMulLess(n);
        try {
            if (testLoopMulLess(n) > 0) {
                throw new Exception("My exception"); 
            }
        } catch (Exception e) {
            res /= testLoopMulLess(n);
        } finally {
            res -= testLoopMulLess(n);
        }
        res *= testLoopMulLess(n);
        return res;
    }

    public static void main(String[] args) {
         System.out.println(Float.floatToIntBits(new Main().$noinline$testTryCatch(1.2f)));
    }
}  

