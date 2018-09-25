
/**
 *
 * Optimization should be applied to the inner loop only
 * 1 sinking excepted for each testcase
 *
 **/


public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar1 = 100;
        int testVar2 = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar1 += 5;
            for (int k = 0; k < iterations; k++) {
                testVar2 += 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar1 = 100;
        int testVar2 = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar1 -= 5;
            for (int k = 0; k < iterations; k++) {
                testVar2 -= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar1 = 100;
        long testVar2 = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar1 -= 5;
            for (long k = 0; k < iterations; k++) {
                testVar2 -= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar1 = 1;
        int testVar2 = 1;
        int additionalVar = 10;

        for (int i = 0; i < 3; i++) {
            testVar1 *= 3;
            for (int k = 0; k < 3; k++) {
                testVar2 *= 6;
                additionalVar += k + k%2 + k%3 + k%4 + k%5 + k%6 +k%7 +k%8 + k%9 + k%10 + n + k*2 - k/3 + k%14 - k*n + k*n*n*n + n%3;
            }
            additionalVar += i + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar1 = 1L;
        long testVar2 = 1L;
        long additionalVar = 10L;

        for (long i = 0; i < 3; i++) {
            testVar1 *= 3;
            for (long k = 0; k < 4; k++) {
                testVar2 *= 6L;
                additionalVar += k + k%2 + k%3 + k%4 + k%5 + k%6 +k%7 +k%8 + k%9 + k%10 + n + k*2 - k/3 + k%14 - k*n + k*n*n*n + n%3;
            }
            additionalVar += i + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar1 = 100;
        int testVar2 = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar1 /= 5;
            for (int k = 0; k < iterations; k++) {
                testVar2 /= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar1 = 100;
        long testVar2 = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar1 /= 5;
            for (long k = 0; k < iterations; k++) {
                testVar2 /= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar1 = 100;
        int testVar2 = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            testVar1 %= 5;
            for (int k = 0; k < iterations; k++) {
                testVar2 %= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar1 = 100;
        long testVar2 = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar1 %= 5;
            for (long k = 0; k < iterations; k++) {
                testVar2 %= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar1 = 100;
        long testVar2 = 100;
        long additionalVar = 10;

        for (long i = 0; i < iterations; i++) {
            testVar1 %= 5;
            for (long k = 0; k < iterations; k++) {
                testVar2 %= 5;
                additionalVar += k;
            }
            additionalVar += i;
        }

        return testVar1 + testVar2 + additionalVar;
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

