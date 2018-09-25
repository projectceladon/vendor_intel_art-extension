
/**
*
* Expected result: 1 sinking because if-else statement is outside the loop
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt(int n) {
        int testVar = 100;
        int additionalVar = 0;
        int setToConstantVar = 3;
    	if (n > 0) {
            for (int i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar += setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public int testLoopSubInt(int n) {
        int testVar = 100;
        int additionalVar = 0;
        int setToConstantVar = 3;
    	if (n > 0) {
            for (int i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar -= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public long testLoopSubLong(long n) {
        long testVar = 100;
        long additionalVar = 0;
        long setToConstantVar = 3;
    	if (n > 0) {
            for (long i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar -= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 0;
        int setToConstantVar = 6;
    	if (n > 0) {
            for (int i = -1; i < 8; i++) {
                additionalVar += n + (i*2)%5 + i%2 - i%3 + n%4 - n%5 + n%6 + i*n;
                testVar *= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar = 0;
        long setToConstantVar = 6L;
    	if (n > 0) {
            for (long i = -3; i < 9; i++) {
                additionalVar += n + (i*2)%5 + i%2 - i%3 + n%4 - n%5 + n%6 + i*n;
                testVar *= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public int testLoopDivInt(int n) {
        int testVar = 40000000;
        int additionalVar = 0;
        int setToConstantVar = 2;
    	if (n > 0) {
            for (int i = -10; i < 15; i++) {
                additionalVar += n +  (i*2)%5;
                testVar /= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public long testLoopDivLong(long n) {
        long testVar = 40000000;
        long additionalVar = 0;
        long setToConstantVar = 2;
    	if (n > 0) {
            for (long i = -10; i < 15; i++) {
                additionalVar += n +  (i*2)%5;
                testVar /= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public int testLoopRemInt(int n) {
        int testVar = 100;
        int additionalVar = 0;
        int setToConstantVar = 3;
    	if (n > 0) {
            for (int i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar %= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public long testLoopRemLong(long n) {
        long testVar = 100;
        long additionalVar = 0;
        long setToConstantVar = 3;
    	if (n > 0) {
            for (long i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar %= setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public long testLoopAddLong(long n) {
        long testVar = 100;
        long additionalVar = 0;
        long setToConstantVar = 3;
    	if (n > 0) {
            for (long i = -10; i < iterations; i++) {
                additionalVar += n +  (i*2)%5;
                testVar += setToConstantVar;
            }
	    } else {
	        testVar = 1000;
	    }
        return testVar + additionalVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(10));
System.out.println(new Main().testLoopAddLong(10));
System.out.println(new Main().testLoopRemLong(10));
System.out.println(new Main().testLoopRemInt(10));
System.out.println(new Main().testLoopDivLong(10));
System.out.println(new Main().testLoopDivInt(10));
System.out.println(new Main().testLoopMulLong(10));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong(10));
System.out.println(new Main().testLoopSubInt(10));
    }

}  

