
/**
 *
 * While loop with count down  IV other than 1
 * L: Expected result: 1 sinking, currently fail due to CAR-876
 * M: Expected result: 1 sinking, currently fail
 * N: no sinking, bound computation still doesn't support count-down loops
 *
 **/


public class Main {                                                                                                                                                   
    final int iterations = 3300;

    public int testLoopAddInt() {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = iterations - 1;
        while (i >= 0 ) {
              testVar += setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = iterations - 1;
        while (i >= 0 ) {
              testVar -= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = iterations - 1;
        while (i >= 0 ) {
              testVar -= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = -1;
        int additionalVar = 0;
        int setToConstantVar = 6;
        int i = 29;
        while (i >= 0 ) {
              testVar *= setToConstantVar;
              additionalVar += i + n + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = -1L;
        long additionalVar = 0L;
        long setToConstantVar = 6L;
        long i = 35;
        while (i >= 0 ) {
              testVar *= setToConstantVar;
              additionalVar += i + n + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopDivInt() {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = iterations - 1;
        while (i >= 0 ) {
              testVar /= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = iterations - 1;
        while (i >= 0 ) {
              testVar /= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = -3017;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = iterations - 1;
        while (i >= 0 ) {
              testVar %= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = iterations - 1;
        while (i >= 0 ) {
              testVar %= setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = -3017;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = iterations - 1;
        while (i >= 0 ) {
              testVar += setToConstantVar;
              additionalVar += i;
              i = i - 3;
        }
        testVar = testVar + additionalVar;
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
System.out.println(new Main().testLoopMulLong(10));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

