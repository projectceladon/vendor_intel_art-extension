
/**
*
* Float-point induction variable
* 1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar += 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public int testLoopSubInt() {
        int testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar -= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public long testLoopSubLong() {
        long testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar -= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < 10f; f = f + 1) {
            testVar *= 6;
            additionalVar = (additionalVar + f)/2 + n + f*n + f*2.5f - f*3.14f + f*1.01f - f*2.12345f + f/5.2f + f*2.1f;
        }

        return testVar + Math.round(additionalVar);
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < 12f; f = f + 1) {
            testVar *= 6L;
            additionalVar = (additionalVar + f)/2 + n + f*n + f*2.5f - f*3.14f + f*1.01f - f*2.12345f + f/5.2f + f*2.1f;
        }

        return testVar + Math.round(additionalVar);
    }

    public int testLoopDivInt() {
        int testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar /= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public long testLoopDivLong() {
        long testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar /= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public int testLoopRemInt() {
        int testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar %= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public long testLoopRemLong() {
        long testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar %= 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public long testLoopAddLong() {
        long testVar = 0xC1C1C1;
        float additionalVar = -1.0f;

        for (float f = 0.0f; f < iterations; f = f + 1) {
            testVar += 5;
            additionalVar = (additionalVar + f)/2;
        }

        return testVar + Math.round(additionalVar);
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt());
System.out.println(new Main().testLoopAddLong());
System.out.println(new Main().testLoopRemLong());
System.out.println(new Main().testLoopRemInt());
System.out.println(new Main().testLoopDivLong());
System.out.println(new Main().testLoopDivInt());
System.out.println(new Main().testLoopMulLong(10));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

