
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    final long testLoopSubLong() {
        long testVar = 0;

        for (int i = 0; i < iterations; i++) {
            testVar -= 5;
        }

        return testVar;
    }

    final long $noinline$testTryCatch() {
        long res = 0;
        try {
            throw new Exception("My exception"); 
        } catch (Exception e) {
        } finally {
            res += testLoopSubLong();
        }
        return res;
    }

    public static void main(String[] args) {
         System.out.println(new Main().$noinline$testTryCatch());
    }
}  

