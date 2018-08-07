
/**
  *
  * No sinking expected because variable is used again in the loop
  *
  **/

public class Main {                                                                                                                                                   
    final int iterations = 1101;

    public int testLoopAddInt() {
        int testVar = 151;

        for (int i = 0; i < iterations; i++) {
            testVar += 5;
            testVar += 5;
        }

        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = 151;

        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
            testVar -= 5;
        }

        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 151;

        for (long i = 0; i < iterations; i++) {
            testVar -= 5;
            testVar -= 5;
        }

        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 1;
        for (int i = 0; i < 4; i++) {
            testVar *= 6;
            testVar *= 6;
            additionalVar += i%2 + i%3 + i%4+ i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n + n*i + n*2 + n%3 - n%4 + n%5 + i*i + n*n;
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 1L;
        for (long i = 0; i < 6; i++) {
            testVar *= 6L;
            testVar *= 6L;
            additionalVar += i%2 + i%3 + i%4+ i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n + n*i + n*2 + n%3 - n%4 + n%5 + i*i + n*n;
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 151;

        for (int i = 0; i < iterations; i++) {
            testVar /= 5;
            testVar /= 5;
        }

        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 151;

        for (long i = 0; i < iterations; i++) {
            testVar /= 5;
            testVar /= 5;
        }

        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = 151;

        for (int i = 0; i < iterations; i++) {
            testVar %= 5;
            testVar %= 5;
        }

        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 151;

        for (long i = 0; i < iterations; i++) {
            testVar %= 5;
            testVar %= 5;
        }

        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 151;

        for (long i = 0; i < iterations; i++) {
            testVar += 5;
            testVar += 5;
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
System.out.println(new Main().testLoopMulLong(10));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

