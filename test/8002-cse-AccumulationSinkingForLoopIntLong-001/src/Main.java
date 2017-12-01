
/**
*
* Checking if Constant Calculation Sinking is applied in combination with Accumulation Sinking optimization
* in the same loop (but no checks for Accumulation Sinking in post-processing)
* 1 constant calculation sinking expected for each test case
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 0;
        int additionalVar = 100;

        for (int i = 0; i < iterations; i++) {
            testVar += 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 0;
        long additionalVar = 100;

        for (long i = 0; i < iterations; i++) {
            testVar += 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 0;
        int additionalVar = 100;

        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 0;
        long additionalVar = 100;

        for (long i = 0; i < iterations; i++) {
            testVar -= 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n ) {

        int testVar = 1;
        int additionalVar1 = 100;
        int additionalVar2 = 200;
        int additionalVar3 = 300;
        int additionalVar4 = 400;
        int additionalVar5 = 500;

        for (int i = 0; i < 10; i++) {
            testVar *= 6;
            additionalVar1 += i + 1;
            additionalVar2 += i + 2;
            additionalVar3 += i + 3;
            additionalVar4 += i + 4;
            additionalVar5 += i + 5 + i % 2 + i % 3 + i % 4 + i % 5 + i % 6 + i % 7 + n;
        }

        return testVar + additionalVar1 + additionalVar2 + additionalVar3 + additionalVar4 + additionalVar5;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar1 = 100;
        long additionalVar2 = 200;
        long additionalVar3 = 300;
        long additionalVar4 = 400;
        long additionalVar5 = 500;

        for (long i = 0; i < 12; i++) {
            testVar *= 6L;
            additionalVar1 += i + 1;
            additionalVar2 += i + 2;
            additionalVar3 += i + 3;
            additionalVar4 += i + 4;
            additionalVar5 += i + 5 + n;
        }

        return testVar + additionalVar1 + additionalVar2 + additionalVar3 + additionalVar4 + additionalVar5;
    }

    public int testLoopDivInt() {
        int testVar = 100;
        int additionalVar = 100;

        for (int i = 0; i < iterations; i++) {
            testVar /= 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 100;
        long additionalVar = 100;

        for (long i = 0; i < iterations; i++) {
            testVar /= 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 100;
        int additionalVar = 100;

        for (int i = 0; i < iterations; i++) {
            testVar %= 5;
            additionalVar += i + 1;
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 100;
        long additionalVar = 100;

        for (long i = 0; i < iterations; i++) {
            testVar %= 5;
            additionalVar += i + 1;
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

