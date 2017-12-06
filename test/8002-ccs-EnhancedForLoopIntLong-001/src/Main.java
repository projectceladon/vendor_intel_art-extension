
/**
*
* L: Enhanced for loop is not supported, 0 sinking expected
* M: Enhanced for loop is supported, 1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar = 100;
        int additionalVar = 1;
        int[] iterator = new int [iterations];
        for (int i : iterator ) {
            additionalVar += i;
            testVar += 5;
        }

        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = 100;
        int additionalVar = 1;
        int[] iterator = new int [iterations];
        for (int i : iterator ) {
            additionalVar += i;
            testVar -= 5;
        }

        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 100;
        long additionalVar = 1;
        long[] iterator = new long [iterations];
        for (long i : iterator ) {
            additionalVar += i;
            testVar -= 5;
        }

        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 1;
        int[] iterator = new int [10];
        for (int i : iterator ) {
            additionalVar += i + n%2 + i%3 + n%4 + i%5 + n%6 + i%7 + n%8 + i%9 + i % 10 + i % 11;
            testVar *= 6;
        }

        return testVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar = 1;
        long[] iterator = new long [18];
        for (long i : iterator ) {
            additionalVar += i + n%2 + i%3 + n%4 + i%5 + n%6 + i%7 + n%8 + i%9 + i % 10 + i % 11;
            testVar *= 6L;
        }

        return testVar;
    }

    public int testLoopDivInt() {
        int testVar = 100;
        int additionalVar = 1;
        int[] iterator = new int [iterations];
        for (int i : iterator ) {
            additionalVar += i;
            testVar /= 5;
        }

        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 100;
        long additionalVar = 1;
        long[] iterator = new long [iterations];
        for (long i : iterator ) {
            additionalVar += i;
            testVar /= 5;
        }

        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = 100;
        int additionalVar = 1;
        int[] iterator = new int [iterations];
        for (int i : iterator ) {
            additionalVar += i;
            testVar %= 5;
        }

        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 100;
        long additionalVar = 1;
        long[] iterator = new long [iterations];
        for (long i : iterator ) {
            additionalVar += i;
            testVar %= 5;
        }

        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 100;
        long additionalVar = 1;
        long[] iterator = new long [iterations];
        for (long i : iterator ) {
            additionalVar += i;
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
System.out.println(new Main().testLoopMulLong(10L));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

