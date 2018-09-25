
/**
 *
 *  Checking what happens if calculation to be sunk overflows
 *  Expected result: if multiplication overflows, no sinking occur 
 *  in order to avoid C++ calculation undefined behavior
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar = Integer.MAX_VALUE - 1;

        for (int j = 1; j < 85; j++) {
            for (int i = 1; i < iterations; i++) {
                testVar += 37;
            }
        }

        return testVar;
    }

    public int testLoopSubInt() {
        int testVar = Integer.MIN_VALUE + 1;;

        for (int j = 1; j < 85; j++) {
            for (int i = 1; i < iterations; i++) {
                testVar -= 37;
            }
        }

        return testVar;
    }

    public long testLoopSubLong() {
        long testVar = Long.MIN_VALUE + 1;

        for (int j = 1; j < 85; j++) {
            for (long i = 1; i < iterations; i++) {
                testVar -= Integer.MAX_VALUE;
            }
        }

        return testVar;
    }

    public int testLoopMulInt(int n) {
        int testVar = -64795;
        int additionalVar = 5;

        for (int j = 1; j < 90; j++) {
            for (int i = 1; i < 30; i++) {
                additionalVar += i/2 + n/2;
                testVar *= 23;
            }
        }

        return testVar;
    }

    public int testLoopOverflowToZeroMulInt() {
        int testVar = -64795;
        for (int i = 1; i < 90; i++) {
            for (int j = 1; j < 30; ++j) {
                testVar *= 22;
            }
        }
        return testVar;
    }

    public long testLoopMulLong() {
        long testVar = Integer.MAX_VALUE;
        for (int j = 0; j < 85; j++) {
            for (int i = 0; i < 37; i++) {
                testVar *= 6;
            }
        }

        return testVar;

    }

    public int testLoopDivInt() {
        int testVar = 100;

        for (int j = 1; j < 85; j++) {
            for (int i = 1; i < iterations; i++) {
                testVar /= 11;
            }
        }

        return testVar;
    }

    public long testLoopDivLong() {
        long testVar = 100;

        for (int j = 2; j < 85; j++) {
            for (long i = 2; i < iterations; i++) {
                testVar /= Long.MIN_VALUE;
            }
        }

        return testVar;
    }

    public int testLoopRemInt() {
        int testVar = Integer.MAX_VALUE;

        for (int j = 4; j < 85; j++) {
            for (int i = 3; i < iterations; i++) {
                testVar %= Integer.MIN_VALUE + 2;
            }
        }

        return testVar;
    }

    public long testLoopRemLong() {
        long testVar = 100;

        for (int j = 5; j < 85; j++) {
            for (long i = 6; i < iterations; i++) {
                testVar %= Long.MIN_VALUE;
            }
        }

        return testVar;
    }

    public long testLoopAddLong() {
        long testVar = Long.MAX_VALUE - 1;

        for (int j = 4; j < 85; j++) {
            for (long i = 3; i < iterations; i++) {
                testVar += Integer.MAX_VALUE - 7;
            }
        }

        return testVar;
    }

    /* 1 sinking: factor overflow to zero; unkown start */
    public int testLoopMulIntUnknownStartZero(int testVar, int n) {

        int additionalVar = 5;
        for (int j = 1; j < 90; j++) {
            for (int i = 1; i < 9; i++) {
                testVar *= 14;
                additionalVar += i/2 + n/2 + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
            }
        }

        return testVar + additionalVar;
    }

    /* 1 sinking: factor doesn't overflow, but testVar*factor does overflow; unkown start */
    public int testLoopMulIntUnknownStart(int testVar, int n) {

        int additionalVar = 5;
            for (int i = 1; i < 9; i++) {
                testVar *= 6;
                additionalVar += i/2 + n/2 + i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9;
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
        System.out.println(new Main().testLoopMulInt(10));
        System.out.println(new Main().testLoopSubLong());
        System.out.println(new Main().testLoopSubInt());
        System.out.println(new Main().testLoopOverflowToZeroMulInt());
        System.out.println(new Main().testLoopMulIntUnknownStartZero(-64795, 10));
        System.out.println(new Main().testLoopMulIntUnknownStart(-64795, 10));
    }

}  

