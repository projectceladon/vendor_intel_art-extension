

/**
*
* LE (less or equal) as loop condition should be supported
* Class instance/static field and local variables set to constant values in the right side of instruction <operation>= should be supported (treated as constant)
* Expected result: 1 sinking for each testcase
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;
    final int constInstanceField = -1;
    final long constInstanceFieldL = -1L;
    final static int constStaticField = -2;
    final static long constStaticFieldL = -2L;

    public int testLoopAddInt() {
        int testVar = 100;
        int setToConstantVar = 5;
        int additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar += setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = 100;
        int setToConstantVar = 5;
        int additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar -= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 100;
        long setToConstantVar = 5;
        long additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar -= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int setToConstantVar = 9;
        int additionalVar = 0;
        for (int i = 0;  10 >= i; i++) {
              testVar *= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5 + n + i%6 + i%4 + i%2 + i%3 + i%5 +i%7 + i%9 + n % 2 + n % 3 + n % 4 + n % 5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long setToConstantVar = 9L;
        long additionalVar = 0;
        for (int i = 0;  12 >= i; i++) {
              testVar *= setToConstantVar + constInstanceFieldL + constStaticFieldL;
              additionalVar += (i*2)%5 + n + i%6 + i%4 + i%2 + i%3 + i%5 +i%7 + i%9 + n % 2 + n % 3 + n % 4 + n % 5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopDivInt() {
        int testVar = 100;
        int setToConstantVar = 5;
        int additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar /= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 100;
        long setToConstantVar = 5;
        long additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar /= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = 100;
        int setToConstantVar = 5;
        int additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar %= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 100;
        long setToConstantVar = 5;
        long additionalVar = 0;
        for (int i = 0;  iterations >= i; i++) {
              testVar %= setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 100;
        long setToConstantVar = 5;
        long additionalVar = 0;
        for (int i = 0;  iterations >= i ; i++) {
              testVar += setToConstantVar + constInstanceField + constStaticField;
              additionalVar += (i*2)%5;
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

