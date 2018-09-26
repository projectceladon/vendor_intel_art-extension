
/**
 *
 * No sinking expected because variable is used again in the loop
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 1000;
        int additionalVar = 0;
        int a = 2;
        
        for (int i = 0; i < iterations; i++) {
            a += testVar;
            testVar += 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public int testLoopSubInt() {
        int testVar = 1000;
        int additionalVar = 0;
        int a = 2;
        
        for (int i = 0; i < iterations; i++) {
            a += testVar;
            testVar -= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public long testLoopSubLong() {
        long testVar = 1000;
        long additionalVar = 0;
        long a = 2;
        
        for (long i = 0; i < iterations; i++) {
            a += testVar;
            testVar -= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 0;
        int a = 2;
        
        for (int i = 0; i < 10; i++) {
            a += testVar;
            testVar *= 6;
            additionalVar += (i*2)%5 + n;
        }

        return testVar + a + additionalVar ;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1L;
        long additionalVar = 0L;
        long a = 2L;
        
        for (long i = 0; i < 12; i++) {
            a += testVar;
            testVar *= 6L;
            additionalVar += (i*2)%5 + n;
        }

        return testVar + a + additionalVar ;
    }

    public int testLoopDivInt() {
        int testVar = 1000;
        int additionalVar = 0;
        int a = 2;
        
        for (int i = 0; i < iterations; i++) {
            a += testVar;
            testVar /= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public long testLoopDivLong() {
        long testVar = 1000;
        long additionalVar = 0;
        long a = 2;
        
        for (long i = 0; i < iterations; i++) {
            a += testVar;
            testVar /= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public int testLoopRemInt() {
        int testVar = 1000;
        int additionalVar = 0;
        int a = 2;
        
        for (int i = 0; i < iterations; i++) {
            a += testVar;
            testVar %= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public long testLoopRemLong() {
        long testVar = 1000;
        long additionalVar = 0;
        long a = 2;
        
        for (long i = 0; i < iterations; i++) {
            a += testVar;
            testVar %= 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
    }

    public long testLoopAddLong() {
        long testVar = 1000;
        long additionalVar = 0;
        long a = 2;
        
        for (long i = 0; i < iterations; i++) {
            a += testVar;
            testVar += 5;
            additionalVar += (i*2)%5;
        }

        return testVar + a + additionalVar ;
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

