

/**
  *
  * L: Break operator doesn't break one basic block limitation for int,
  * for some reasons (most probably not a bug), there are two basic blocks for long type,
  * 1 sinking expected for int, 0 for long.
  * M: no limitations on basic blocks number, 1 constant calculation sinking expected for
  * each method
  *
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar = 10000;
        int additionalVar = 10;

outer:  
        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
         for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar += 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 10000;
        int additionalVar = 10;

outer:  
        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
         for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar -= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 10000;
        long additionalVar = 10;

outer:  
        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
         for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar -= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = 1;
        int additionalVar = 10;

outer:  
        for (int i = 0; i < 3; i++) {
            additionalVar += i + n*2;
         for (int k = 0; k < 5; k++) {
                additionalVar += k + n + k%3 - i%2 + n%4 - i%5 + (k+2)/7 - (i - 5)/3 + k*3 - k/2;
                testVar *= 6;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long testVar = 1;
        long additionalVar = 10;

outer:  
        for (long i = 0; i < 5; i++) {
            additionalVar += i + n;
         for (long k = 0; k < 5; k++) {
                additionalVar += k + n + k%3 - i%2 + n%4 - i%5 + (k+2)/7 - (i - 5)/3 + k*3 - k/2;
                testVar *= 6L;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 10000;
        int additionalVar = 10;

outer:  
        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
         for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar /= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 10000;
        long additionalVar = 10;

outer:  
        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
         for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar /= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 10000;
        int additionalVar = 10;

outer:  
        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
         for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar %= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 10000;
        long additionalVar = 10;

outer:  
        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
         for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar %= 5;
	            continue outer;
            }
        }

        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 10000;
        long additionalVar = 10;

outer:  
        for (long i = 0; i < iterations; i++) {
            additionalVar += i;
         for (long k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar += 5;
	            continue outer;
            }
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

