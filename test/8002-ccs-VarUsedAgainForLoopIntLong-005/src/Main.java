
/**
 *
 * No sinking expected because variable is used again in the loop
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopInt() {
        int testVar = 387;

        for (int i = 0; i < iterations; i++) {
            testVar += 118;
            testVar /= 31;
            testVar *= 6;
            testVar -= -10;
//            testVar %= 2;
        }

        return testVar;
    }

    public long testLoopLong() {
        long testVar = 387;

        for (int i = 0; i < iterations; i++) {
            testVar += 118;
            testVar /= 31;
            testVar *= 6L;
            testVar -= -10;
//            testVar %= 2;
        }

        return testVar;
    }

 
    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopInt());
         System.out.println(new Main().testLoopLong());
    }

}  

