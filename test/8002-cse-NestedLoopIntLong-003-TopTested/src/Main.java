
/**
 *
 * Optimization should be applied to the inner loop only
 * 1 sinking excepted for each testcase
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt(int n) {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar += 5;
            for (int k = 0; k < iterations; k++) {
                for (int j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar += 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt(int n) {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
            for (int k = 0; k < iterations; k++) {
                for (int j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar -= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong(long n) {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar -= 5;
            for (long k = 0; k < iterations; k++) {
                for (long j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar -= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n ) {
        int testVar = 1;
        int additionalVar = 10;

        for (int i = 0; i < 2; i++) {
            testVar *= 3;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
            for (int k = 0; k < 2; k++) {
                additionalVar += k%2 + k%3 + k%4 + k%5 + k%6 + k%7 + k%8 + k%9 + k%10 + n;
                for (int j = 0; j < 2; j++) {
                    additionalVar += j%2 + j%3 + j%4 + j%5 + j%6 + j%7 + j%8 + j%9 + j%10 + n + j*n + n%2 + n%3 + n%4 + n%5 + n%6 + n%7 + n%8;
                    testVar *= 6;
                }
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 10L;

        for (long i = 0; i < 2; i++) {
            testVar *= 3;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
            for (long k = 0; k < 2; k++) {
                    additionalVar += k%2 + k%3 + k%4 + k%5 + k%6 + k%7 + k%8 + k%9 + k%10 + n;
                for (long j = 0; j < 3; j++) {
                    additionalVar += j%2 + j%3 + j%4 + j%5 + j%6 + j%7 + j%8 + j%9 + j%10 + n + j*n + n%2 + n%3 + n%4 + n%5 + n%6 + n%7 + n%8;
                    testVar *= 6L;
                }
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt(int n ) {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar /= 5;
            for (int k = 0; k < iterations; k++) {
                for (int j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar /= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong(long n ) {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar /= 5;
            for (long k = 0; k < iterations; k++) {
                for (long j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar /= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt(int n) {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar %= 5;
            for (int k = 0; k < iterations; k++) {
                for (int j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar %= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong(long n) {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar %= 5;
            for (long k = 0; k < iterations; k++) {
                for (long j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar %= 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong(long n) {
        long testVar = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar += 5;
            for (long k = 0; k < iterations; k++) {
                for (long j = 0; j < iterations; j++) {
                    additionalVar += k + n;
                    testVar += 2;
                }
            }
        }

        return testVar + additionalVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(10));
System.out.println(new Main().testLoopAddLong(10L));
System.out.println(new Main().testLoopRemLong(10L));
System.out.println(new Main().testLoopRemInt(10));
System.out.println(new Main().testLoopDivLong(10L));
System.out.println(new Main().testLoopDivInt(10));
System.out.println(new Main().testLoopMulLong(10L));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong(10L));
System.out.println(new Main().testLoopSubInt(10));
    }

}  

