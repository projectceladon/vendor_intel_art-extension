

/**
* 
* Two induction variables
* 1 sinking expected for each test case
*
*/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 0x1234;
        int additionalVar = -1;

        for (int i = 0, k=5; i < iterations; i++, k++) {
            testVar += 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 0x1234;
        int additionalVar = -1;

        for (int i = 0, k=5; i < iterations; i++, k++) {
            testVar -= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 0x1234;
        long additionalVar = -1;

        for (long i = 0, k=5; i < iterations; i++, k++) {
            testVar -= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = -1;

        for (int i = 0, k=5; i < 11; i++, k++) {
            testVar *= 6;
            additionalVar = (additionalVar + k)%2 + n + i%2 - k%3 + i%4 + k%5 - n%6 + i%7 + k%8 - i%9;
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar = -1;

        for (long i = 0, k=5; i < 12; i++, k++) {
            testVar *= 6L;
            additionalVar = (additionalVar + k)%2 + n + i%2 - k%3 + i%4 + k%5 - n%6 + i%7 + k%8 - i%9;
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 0x1234;
        int additionalVar = -1;

        for (int i = 0, k=5; i < iterations; i++, k++) {
            testVar /= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 0x1234;
        long additionalVar = -1;

        for (long i = 0, k=5; i < iterations; i++, k++) {
            testVar /= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 0x1234;
        int additionalVar = -1;

        for (int i = 0, k=5; i < iterations; i++, k++) {
            testVar %= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 0x1234;
        long additionalVar = -1;

        for (long i = 0, k=5; i < iterations; i++, k++) {
            testVar %= 5;
            additionalVar = (additionalVar + k)%2;
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 0x1234;
        long additionalVar = -1;

        for (long i = 0, k=5; i < iterations; i++, k++) {
            testVar += 5;
            additionalVar = (additionalVar + k)%2;
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
System.out.println(new Main().testLoopMulLong(10));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

