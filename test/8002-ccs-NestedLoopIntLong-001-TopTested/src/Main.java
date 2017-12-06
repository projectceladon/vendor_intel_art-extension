
/**
 *
 * Optimization should be applied to the inner loop only
 * 1 sinking excepted for each testcase
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
            for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar += 5;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
            for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar -= 5;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
            for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar -= 5;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 10;

        for (int i = 0; i < 3; i++) {
            additionalVar += i;
            for (int k = 0; k < 3; k++) {
                additionalVar += k + n%2 + k%3 + n%4 + k%5 + n%6 + k%7 + n%8 + i%9 + i*n*k + i*k%3 + n*i%4 + n%3 + k%4 + i%5 + k%6 + n%9 - i%8;
                testVar *= 6;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 10;

        for (long i = 0; i < 3; i++) {
            additionalVar += i;
            for (long k = 0; k < 4; k++) {
                additionalVar += k + n%2 + k%3 + n%4 + k%5 + n%6 + k%7 + n%8 + i%9 + i*n*k + i*k%3 + n*i%4 + n%3 + k%4 + i%5 + k%6 + n%9 - i%8;
                testVar *= 6L;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
            for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar /= 5;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
            for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar /= 5;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
            for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar %= 5;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
            for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar %= 5;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
            for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar += 5;
            }
        }

        return testVar + additionalVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt());
System.out.println(new Main().testLoopAddLong());
System.out.println(new Main().testLoopRemLong());
System.out.println(new Main().testLoopRemInt());
System.out.println(new Main().testLoopDivLong());
System.out.println(new Main().testLoopDivInt());
System.out.println(new Main().testLoopMulLong(10L));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

