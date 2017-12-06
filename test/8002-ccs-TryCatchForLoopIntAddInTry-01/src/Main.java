
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    final int testLoopAddInt() {
        int testVar = 0;

        for (int i = 0; i < iterations; i++) {
            testVar += 5;
        }

        return testVar;
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        try {
            if (testLoopAddInt() > 0) {
                throw new Exception("My exception"); 
            }
        } catch (Exception e) {
        } finally {
        }
        return res;
    }

    public static void main(String[] args) {
         System.out.println(new Main().$noinline$testTryCatch());
    }
}  

