
/**
 *
 * While loop with IV other than 1 and condition that doesn't breaks limitation
 * one operation per iteration
 * Expected result: 1 sinking
 *
 **/


public class Main {                                                                                                                                                   
    final int iterations = 3300;
//    final boolean condition = false;

    public int testLoopAddInt(boolean condition) {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
            testVar += setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
            i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopSubInt(boolean condition) {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
            testVar -= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
            i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopSubLong(boolean condition) {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
            testVar -= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
           i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopMulInt(int n, boolean condition) {
        int testVar = -1;
        int additionalVar = 0;
        int setToConstantVar = 6;
        int i = 0;
        while (i < 29 ) {
            testVar *= setToConstantVar;
            if (condition) {
                additionalVar += i + n;
            }
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopMulLong(long n, boolean condition) {
        long testVar = -1L;
        long additionalVar = 0L;
        long setToConstantVar = 6L;
        long i = 0;
        while (i < 35 ) {
            testVar *= setToConstantVar;
            if (condition) {
                additionalVar += i + n;
            }
            i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopDivInt(boolean condition) {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
            testVar /= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
            i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopDivLong(boolean condition) {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
            testVar /= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopRemInt(boolean condition) {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
            testVar %= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopRemLong(boolean condition) {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
            testVar %= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopAddLong(boolean condition) {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
            testVar %= setToConstantVar;
            if (condition) {
                additionalVar += i;
            }
                          i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(false));
System.out.println(new Main().testLoopAddLong(false));
System.out.println(new Main().testLoopRemLong(false));
System.out.println(new Main().testLoopRemInt(false));
System.out.println(new Main().testLoopDivLong(true));
System.out.println(new Main().testLoopDivInt(true));
System.out.println(new Main().testLoopMulLong(10, false));
System.out.println(new Main().testLoopMulInt(10, false));
System.out.println(new Main().testLoopSubLong(true));
System.out.println(new Main().testLoopSubInt(true));
    }

}  

