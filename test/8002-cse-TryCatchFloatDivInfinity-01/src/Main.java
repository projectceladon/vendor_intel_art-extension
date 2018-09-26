
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterationsGreaterThanThreshold = 70;

    final float testLoopDivInfinity(float n) {
        float testVar = 1.0f;
        float additionalVar = 0;
        float smallVar = 10/Float.MAX_VALUE;
        for (int i = -10; i < iterationsGreaterThanThreshold; i++) {
              testVar /= smallVar;
              additionalVar += (i*2)/5 + n;
        }
        testVar += additionalVar;
        return testVar;
    }

    final float $noinline$testTryCatch(float n) {
        float res = 0;
        res += testLoopDivInfinity(n);
        try {
            if (testLoopDivInfinity(n) > 0) {
                throw new Exception("My exception"); 
            }
        } catch (Exception e) {
            res /= testLoopDivInfinity(n);
        } finally {
            res -= testLoopDivInfinity(n);
        }
        res *= testLoopDivInfinity(n);
        return res;
    }

    public static void main(String[] args) {
         System.out.println(Float.floatToIntBits(new Main().$noinline$testTryCatch(1.2f)));
    }
}  

