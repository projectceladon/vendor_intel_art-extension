
/**
*
* No sinking expected: according to update from Jc only local variable can be sunk
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int[] testVar = new int[] {100, 200, 300};

        for (int i = 0; i < iterations; i++) {
            testVar[0] += 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public int testLoopSubInt() {
        int[] testVar = new int[] {100, 200, 300};

        for (int i = 0; i < iterations; i++) {
            testVar[0] -= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public long testLoopSubLong() {
        long[] testVar = new long[] {100, 200, 300};

        for (long i = 0; i < iterations; i++) {
            testVar[0] -= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public int testLoopMulInt() {
        int[] testVar = new int[] {1, 200, 300};
        int additionalVar = 0;
        for (int i = 0; i < 9; i++) {
            testVar[0] *= 6;
            additionalVar += i/2 + i/3 + i%4 + i%5 + i%6 + i%8 + i%8 + i%9 + i%10;
        }

        return testVar[0] + testVar[1] + testVar[2] + additionalVar;
    }

    public long testLoopMulLong() {
        long[] testVar = new long[] {1, 200, 300};
        int additionalVar = 0;
        for (long i = 0; i < 25; i++) {
            testVar[0] *= 6L;
            additionalVar += i/2;
        }

        return testVar[0] + testVar[1] + testVar[2] + additionalVar;
    }

    public int testLoopDivInt() {
        int[] testVar = new int[] {100000, 200, 300};

        for (int i = 0; i < iterations; i++) {
            testVar[0] /= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public long testLoopDivLong() {
        long[] testVar = new long[] {100000, 200, 300};

        for (long i = 0; i < iterations; i++) {
            testVar[0] /= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public int testLoopRemInt() {
        int[] testVar = new int[] {100, 200, 300};

        for (int i = 0; i < iterations; i++) {
            testVar[0] %= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public long testLoopRemLong() {
        long[] testVar = new long[] {100, 200, 300};

        for (long i = 0; i < iterations; i++) {
            testVar[0] %= 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
    }

    public long testLoopAddLong() {
        long[] testVar = new long[] {100, 200, 300};

        for (long i = 0; i < iterations; i++) {
            testVar[0] += 5;
        }

        return testVar[0] + testVar[1] + testVar[2];
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

