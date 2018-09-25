
/**
*
* No sinking expected: loop has unknown number of iterations
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt(int n) {
        int testVar = 10;
        int additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
            testVar += 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                System.out.println("Exception " + ae);
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt(int n) {
        int testVar = 10;
        int additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong(long n) {
        long testVar = 10;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
            testVar -= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 0;
        for (int i = 0; i < 10; i++) {
            testVar *= 6;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 0;
        for (long i = 0; i < 12; i++) {
            testVar *= 6L;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt(int n) {
        int testVar = 10;
        int additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
            testVar /= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong(long n) {
        long testVar = 10;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
            testVar /= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt(int n) {
        int testVar = 10;
        int additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
            testVar %= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong(long n) {
        long testVar = 10;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
            testVar %= 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong(long n) {
        long testVar = 10;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
            testVar += 5;
            try {
               throw new java.lang.ArithmeticException();
            } catch (java.lang.ArithmeticException ae) {
//                ae.printStackTrace();
            }
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + n;
        }

        return testVar + additionalVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(10));
System.out.println(new Main().testLoopAddLong(10L));
System.out.println(new Main().testLoopRemLong(10L));
System.out.println(new Main().testLoopRemInt(10));
System.out.println(new Main().testLoopDivLong(10L));
System.out.println(new Main().testLoopDivInt(10));
System.out.println(new Main().testLoopMulLong(10L));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong(10L));
System.out.println(new Main().testLoopSubInt(10));

    }

}  

