
/**
 *
 *  Covering more multiplication cases
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 2000;
 
    private int inlineIntegerConst(int x) {
        return x;
    }


/* These tests are not relevant for M because multiplication by 0, 1, -1 is optimized by previous pass

    public int testLoop1MulInt() {
        int testVar = 123456;
        // to avoid javac optimization
        int operand = inlineIntegerConst(3);
        for (int i = 0; i < iterations; i++) {
            testVar *= operand;
        }
        return testVar;
    }

    public int testLoopNeg1MulInt() {
        int testVar = 123456;
        for (int i = 0; i < iterations; i++) {
            TEStVar *= -1;
        }
        return testVar;
    }

    public int testLoopZeroMulInt() {
        int testVar = 123456;
        for (int i = 0; i < iterations; i++) {
            testVar *= 0;
        }
        return testVar;
    }
     public long testLoop1MulLong() {
        long testVar = 123456L;
        for (long i = 0; i < iterations; i++) {
            testVar *= 1L;
        }
        return testVar;
    }

    public long testLoopNeg1MulLong() {
        long testVar = 123456L;
        for (long i = 0; i < iterations; i++) {
            testVar *= -1L;
        }
        return testVar;
    }

    public long testLoopZeroMulLong() {
        long testVar = 123456L;
        for (long i = 0; i < iterations; i++) {
            testVar *= 0L;
        }
        return testVar;
    }



*/

    public int testLoopOverflowMulInt() {
        int testVar = -64795;

        for (int i = 1; i < iterations; i++) {
                testVar *= 25;
        }

        return testVar;
    }

    public int testLoopOverflowMaxMulInt() {
        int testVar = Integer.MAX_VALUE;

        for (int i = 1; i < iterations; i++) {
                testVar *= 25;
        }

        return testVar;
    }

    public int testLoopOverflowMinMulInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 1; i < iterations; i++) {
                testVar *= 25;
        }

        return testVar;
    }

    /* Overflow to zero */
    public int testLoopOverflowMin1MulInt(int n) {
        int testVar = -1*Integer.MIN_VALUE/4;
        int additionalVar = 0;

        for (int i = 0; i < 35; i++) {
                testVar *= -6;
                additionalVar += n%3;
        }

        return testVar + additionalVar;
    }

    /* Overflow to zero */
    public long testLoopOverflowMin1MulLong(long n) {
        long testVar = (long)-1*Long.MIN_VALUE/4;
        long additionalVar = 0L;

        for (long i = 0; i < 135; i++) {
                testVar *= -6L;
                additionalVar += n%3;
        }

        return testVar;// + additionalVar;
    }


    public long testLoopOverflowMaxMulLong() {
        long testVar = Long.MAX_VALUE;

        for (int i = 1; i < iterations; i++) {
                testVar *= 25L;
        }

        return testVar;
    }

    public long testLoopOverflowMinMulLong() {
        long testVar = Long.MIN_VALUE;

        for (int i = 1; i < iterations; i++) {
                testVar *= 25L;
        }

        return testVar;
    }



    public int testLoopOverflowToZeroNegMulInt() {
        int testVar = -64795;

        for (int i = 1; i < iterations; i++) {
                testVar *= 22;
        }

        return testVar;
    }

    public int testLoopOverflowToZeroPosMulInt() {
        int testVar = 64795;

        for (int i = 1; i < iterations; i++) {
                testVar *= 22;
        }

        return testVar;
    }


    public long testLoopOverflowToZeroNegMulLong() {
        long testVar = -64795L;

        for (int i = 1; i < iterations; i++) {
                testVar *= 22L;
        }

        return testVar;
    }

    public long testLoopOverflowToZeroPosMulLong() {
        long testVar = 64795L;

        for (int i = 1; i < iterations; i++) {
                testVar *= 22L;
        }

        return testVar;
    }



   
    public long testLoopOverflowMulLong() {
        long testVar = 123456L;
        for (int i = 0; i < iterations; i++) {
            testVar *= 25L;
        }
        return testVar;

    }


    public static void main(String[] args)
    {

/* These tests are not relevant for M because multiplication by 0, 1, -1 is optimized by previous pass
        System.out.println(new Main().testLoop1MulInt());
        System.out.println(new Main().testLoopNeg1MulInt());
        System.out.println(new Main().testLoopZeroMulInt());
        System.out.println(new Main().testLoopOverflowMulInt());
        System.out.println(new Main().testLoop1MulLong());
        System.out.println(new Main().testLoopNeg1MulLong());
        System.out.println(new Main().testLoopZeroMulLong());
*/        
        System.out.println(new Main().testLoopOverflowMulLong());
        System.out.println(new Main().testLoopOverflowToZeroNegMulInt());
        System.out.println(new Main().testLoopOverflowToZeroNegMulLong());
        System.out.println(new Main().testLoopOverflowToZeroPosMulInt());
        System.out.println(new Main().testLoopOverflowToZeroPosMulLong());
        System.out.println(new Main().testLoopOverflowMaxMulInt());
        System.out.println(new Main().testLoopOverflowMinMulInt());
        System.out.println(new Main().testLoopOverflowMaxMulLong());
        System.out.println(new Main().testLoopOverflowMinMulLong());
        System.out.println(new Main().testLoopOverflowMin1MulInt(10));
        System.out.println(new Main().testLoopOverflowMin1MulLong(10));

    }

}  

