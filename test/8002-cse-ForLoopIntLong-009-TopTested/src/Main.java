
/**
*
* was: Count-down loops should be supported, now fails (L: CAR-876)
* was: Expected result: 1 sinking
* N: no sinking, bound computation still doesn't support count-down loops
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 0x100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = iterations - 1; i>=0; --i) {
              testVar += setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar += additionalVar;
        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = 0x100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = iterations - 1; i>=0; --i) {
              testVar -= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar -= additionalVar;
        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 0x100;
        long additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = iterations - 1; i>=0; --i) {
              testVar -= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar -= additionalVar;
        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 0;
        int setToConstantVar = 6;
        for (int i = 10; i>=0; --i) {
              testVar *= setToConstantVar;
              additionalVar += (i*2)%5 + n + i%2 - n%3 + i%4 - n%5 + i%6 - n%7 + i%8 - n%9;
        }
        testVar += additionalVar;
        return testVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar = 0;
        long setToConstantVar = 6L;
        for (long i = 12; i>=0; --i) {
              testVar *= setToConstantVar;
              additionalVar += (i*2)%5 + n + i%2 - n%3 + i%4 - n%5 + i%6 - n%7 + i%8 - n%9;
        }
        testVar += additionalVar;
        return testVar;
    }

    public int testLoopDivInt() {
        int testVar = 0x100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = iterations - 1; i>=0; --i) {
              testVar /= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar /= additionalVar;
        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 0x100;
        long additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = iterations - 1; i>=0; --i) {
              testVar /= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar /= additionalVar;
        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = 0x100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = iterations - 1; i>=0; --i) {
              testVar %= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar += additionalVar;
        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 0x100;
        long additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = iterations - 1; i>=0; --i) {
              testVar %= setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar += additionalVar;
        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 0x100;
        long additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = iterations - 1; i>=0; --i) {
              testVar += setToConstantVar;
              additionalVar += (i*2)%5;
        }
        testVar += additionalVar;
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

