
import java.util.Random;

/**
*
* L: Function invocation (if not inlined) breaks one basic block limitation of
* L: Constant Calculation Sinking optimization
* L: Expected result: no sinking expected
* M, N: No limitations on basic blocks number, function invocation allowed
* M, N: Expected result: 1 sinking for each method (if var to be sunk is not passed to function as parameter)
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int foo(long n) {
        int s = 0;
        Random rand = new Random();
        for (int i = 0; i < n; i++) {
            s += rand.nextInt(10);

        }
        return s;
    }

    public int testLoopAddInt() {
        int testVar = 10000;
        int additionalVar = 0;

        for (int i = 0; i < iterations; i++) {
            testVar += 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public int testLoopSubInt() {
        int testVar = 10000;
        int additionalVar = 0;

        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public long testLoopSubLong() {
        long testVar = 10000;
        long additionalVar = 0;

        for (long i = 0; i < iterations; i++) {
            testVar -= 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public int testLoopMulInt() {
        int testVar = 1;
        int additionalVar = 0;

        for (int i = 0; i < 10; i++) {
            testVar *= 6;
            additionalVar += foo(i) + i%2 - i%3 + i%4 - i%5 + i%6 - i%7 + i%8 - i%9 + i%10;
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public long testLoopMulLong() {
        long testVar = 1;
        long additionalVar = 0;

        for (long i = 0; i < 12; i++) {
            testVar *= 6L;
            additionalVar += foo(i) + i%2 - i%3 + i%4 - i%5 + i%6 - i%7 + i%8 - i%9 + i%10;
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public int testLoopDivInt() {
        int testVar = 100000;
        int additionalVar = 0;

        for (int i = 0; i < 25; i++) {
            testVar /= 2;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public long testLoopDivLong() {
        long testVar = 100000;
        long additionalVar = 0;

        for (long i = 0; i < 25; i++) {
            testVar /= 2;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public int testLoopRemInt() {
        int testVar = 10000;
        int additionalVar = 0;

        for (int i = 0; i < iterations; i++) {
            testVar %= 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public long testLoopRemLong() {
        long testVar = 10000;
        long additionalVar = 0;

        for (long i = 0; i < iterations; i++) {
            testVar %= 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }

    public long testLoopAddLong() {
        long testVar = 10000;
        long additionalVar = 0;

        for (long i = 0; i < iterations; i++) {
            testVar += 5;
            additionalVar += foo(i);
        }

        return testVar + additionalVar/(iterations*iterations*10*2) ;
    }


    private static int largest(int... numbers) {
        int currentLargest = numbers[0];
        for (int number : numbers) {
            if (number > currentLargest) {
                currentLargest = number;
            }
        }
        return currentLargest;
    }


    public int testLoopVarargs() {
        int testVar = 100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
            testVar += 5;
            additionalVar += (i*2)%5 + largest(0, i, 1, i, additionalVar, testVar);
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
System.out.println(new Main().testLoopVarargs());
    }

}  

