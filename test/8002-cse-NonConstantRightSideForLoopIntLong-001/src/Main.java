
/**
  *
  * Non-constant right side is not supported even if it's invariant
  * Expected result: no sinking
  **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt(int additionalVar) {
        int testVar = 1000;
        for (int i = 0; i < iterations; i++) {
              testVar += additionalVar;
        }
        return testVar;
    }

    public int testLoopSubInt(int additionalVar) {
        int testVar = 1000;
        for (int i = 0; i < iterations; i++) {
              testVar -= additionalVar;
        }
        return testVar;
    }

    public long testLoopSubLong(long additionalVar) {
        long testVar = 1000;
        for (long i = 0; i < iterations; i++) {
              testVar -= additionalVar;
        }
        return testVar;
    }

    public int testLoopMulInt(int additionalVar) {
        int testVar = 1;
        int s = 0;
        for (int i = 0; i < 9; i++) {
              testVar *= additionalVar;
              s += i%2 + i%3 + i%4 + i%5 + i%6 + i*i + i*i*i*i + i%7 + i%8 + i%9 + i%10;
        }
        return testVar + s;
    }

    public long testLoopMulLong(long additionalVar) {
        long testVar = 1;
        int s = 0;
        for (long i = 0; i < 12; i++) {
              testVar *= additionalVar;
              s += i%2 + i%3 + i%4 + i%5 + i%6 + i*i + i*i*i*i + i%7 + i%8 + i%9 + i%10;
        }
        return testVar + s;
    }

    public int testLoopDivInt(int additionalVar) {
        int testVar = 1000;
        for (int i = 0; i < iterations; i++) {
              testVar /= additionalVar;
        }
        return testVar;
    }

    public long testLoopDivLong(long additionalVar) {
        long testVar = 1000;
        for (long i = 0; i < iterations; i++) {
              testVar /= additionalVar;
        }
        return testVar;
    }

    public int testLoopRemInt(int additionalVar) {
        int testVar = 1000;
        for (int i = 0; i < iterations; i++) {
              testVar %= additionalVar;
        }
        return testVar;
    }

    public long testLoopRemLong(long additionalVar) {
        long testVar = 1000;
        for (long i = 0; i < iterations; i++) {
              testVar %= additionalVar;
        }
        return testVar;
    }

    public long testLoopAddLong(long additionalVar) {
        long testVar = 1000;
        for (long i = 0; i < iterations; i++) {
              testVar += additionalVar;
        }
        return testVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(5));
System.out.println(new Main().testLoopAddLong(5));
System.out.println(new Main().testLoopRemLong(5));
System.out.println(new Main().testLoopRemInt(5));
System.out.println(new Main().testLoopDivLong(5));
System.out.println(new Main().testLoopDivInt(5));
System.out.println(new Main().testLoopMulLong(6L));
System.out.println(new Main().testLoopMulInt(6));
System.out.println(new Main().testLoopSubLong(5));
System.out.println(new Main().testLoopSubInt(5));
    }

}  

