
/**
  *
  * While loop with IV other than 1
  * Expected result: 1 sinking for each test method
  *
  **/

public class Main {                                                                                                                                                   
    final int iterations = 3100;

    public int testLoopAddInt() {
        int testVar = 1002;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
              testVar += setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = 1002;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
              testVar -= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 1002;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
              testVar -= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 0;
        int setToConstantVar = 6;
        int i = 1;
        while (i <= 29 ) {
              testVar *= setToConstantVar;
              additionalVar += i + n + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 0L;
        long setToConstantVar = 6L;
        long i = 1;
        while (i <= 36 ) {
              testVar *= setToConstantVar;
              additionalVar += i + n + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopDivInt() {
        int testVar = 1002;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
              testVar /= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 1002;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
              testVar /= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = 1002;
        int additionalVar = 0;
        int setToConstantVar = 3;
        int i = 0;
        while (i < iterations ) {
              testVar %= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 1002;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
              testVar %= setToConstantVar;
              additionalVar += i;
              i = i + 3;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 1002;
        long additionalVar = 0;
        long setToConstantVar = 3;
        long i = 0;
        while (i < iterations ) {
              testVar += setToConstantVar;
              additionalVar += i;
              i = i + 3;
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

