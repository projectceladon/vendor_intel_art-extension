

/**
*
* 1 sinking expected: basic test case, large loop count
*
**/

public class Main {                                                                                                                                                   
    final int iterations = Integer.MAX_VALUE;

    public int testLoopAddInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 0; i < iterations; i++) {
            testVar += 2;
        }

        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 0; i < iterations; i++) {
            testVar -= 2;
        }

        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = Long.MAX_VALUE;

        for (long i = 0; i < iterations; i++) {
            testVar -= 2;
        }

        return testVar;
    }

    /*
     * can't check multiplication with large loop count
     *
    public int testLoopMulInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 0; i < iterations; i++) {
            testVar *= 2;
        }

        return testVar;
    }

    public long testLoopMulLong() {
        long testVar = Integer.MIN_VALUE;

        for (long i = 0; i < iterations; i++) {
            testVar *= 2;
        }

        return testVar;
    }
    *
    */

    public int testLoopDivInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 0; i < iterations; i++) {
            testVar /= 2;
        }

        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = Long.MIN_VALUE;

        for (long i = 0; i < iterations; i++) {
            testVar /= 2;
        }

        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = Integer.MIN_VALUE;

        for (int i = 0; i < iterations; i++) {
            testVar %= 2;
        }

        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = Long.MIN_VALUE;

        for (long i = 0; i < iterations; i++) {
            testVar %= 2;
        }

        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = Long.MIN_VALUE;

        for (long i = 0; i < iterations; i++) {
            testVar += 2;
        }

        return testVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt());
System.out.println(new Main().testLoopAddLong());
System.out.println(new Main().testLoopRemLong());
System.out.println(new Main().testLoopRemInt());
System.out.println(new Main().testLoopDivLong());
System.out.println(new Main().testLoopDivInt());
//System.out.println(new Main().testLoopMulLong());
//System.out.println(new Main().testLoopMulInt());
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

