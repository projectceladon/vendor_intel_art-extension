
/**
  *
  * 1 sinking expected: loop is inlined inside try-catch block, has known number of iterations
  *
  **/

public class Main {                                                                                                                                                   
    final static int iterations = 2000;


    static int inlinedAddInt(int testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar += 5;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }

    static long inlinedAddLong(long testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar += 5L;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }



    static int inlinedSubInt(int testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar -= 5;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }


    static long inlinedSubLong(long testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar -= 5L;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6;
       }
       return testVar + additionalVar;
    }

    static int inlinedMulInt(int testVar, int n) {
        int additionalVar = 1;
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 1; i++) {
                testVar *= 6;
                additionalVar += i + i*n;
            }
        }
        return testVar + additionalVar;
    }

    static long inlinedMulLong(long testVar, int n) {
        int additionalVar = 1;
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 1; i++) {
                testVar *= 6L;
                additionalVar += i + n;
            }
        }
        return testVar + additionalVar;
    }

    static int inlinedRemInt(int testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar %= 5;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }

    static long inlinedRemLong(long testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar %= 5L;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }

    static int inlinedDivInt(int testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar /= 5;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }

    static long inlinedDivLong(long testVar) {
        int additionalVar = 1;
        for (int i = 0; i < iterations; i++) {
           testVar /= 5L;
           additionalVar += i + 2*i - i*7 + i*4 + i*5 + i*6*i;
       }
       return testVar + additionalVar;
    }



    public int testLoopAddInt() {
        int testVar = 10;

        try {
            testVar = inlinedAddInt(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = 10L;

        try {
            testVar = inlinedAddLong(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }


    public int testLoopSubInt() {
        int testVar = 10;

        try {
            testVar = inlinedSubInt(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = 10L;

        try {
            testVar = inlinedSubLong(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

    public int testLoopMulInt() {
        int testVar = 10;

        try {
            testVar = inlinedMulInt(testVar, 10);
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

    public long testLoopMulLong() {
        long testVar = 10L;

        try {
            testVar = inlinedMulLong(testVar, 10);
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

     public int testLoopRemInt() {
        int testVar = 10;

        try {
            testVar = inlinedRemInt(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

     public long testLoopRemLong() {
        long testVar = 10L;

        try {
            testVar = inlinedRemLong(testVar);
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

     public int testLoopDivInt() {
        int testVar = 10;

        try {
            testVar = inlinedDivInt(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
        }

        return testVar;
    }

     public long testLoopDivLong() {
        long testVar = 10L;

        try {
            testVar = inlinedDivLong(testVar);;
        } catch (java.lang.ArithmeticException ae) {
           System.out.println(ae.getClass());
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
System.out.println(new Main().testLoopMulLong());
System.out.println(new Main().testLoopMulInt());
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

