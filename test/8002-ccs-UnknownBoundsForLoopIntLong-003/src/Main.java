
import java.util.Random;

/**
 *
 * Bounds unknown at compile time
 * No sinking expected
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar = 23;
        int additionalVar = 14;
        for (int i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar += 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 23;
        int additionalVar = 14;
        for (int i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar -= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 23;
        long additionalVar = 14;
        for (long i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar -= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt() {
        int testVar = 1;
        int additionalVar = 14;
        for (int i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar *= 6;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong() {
        long testVar = 1L;
        long additionalVar = 14;
        for (long i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar *= 6L;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 23;
        int additionalVar = 14;
        for (int i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar /= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 23;
        long additionalVar = 14;
        for (long i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar /= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 23;
        int additionalVar = 14;
        for (int i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar %= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 23;
        long additionalVar = 14;
        for (long i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar %= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 23;
        long additionalVar = 14;
        for (long i = 0; (i < iterations) && (testVar < 100); i++) {
            testVar += 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6;
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
System.out.println(new Main().testLoopMulLong());
System.out.println(new Main().testLoopMulInt());
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

