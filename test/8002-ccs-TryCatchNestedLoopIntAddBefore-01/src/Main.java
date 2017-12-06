
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    final int testLoopAddInt() {
        int testVar = 100;
        int additionalVar = 10;

        for (int i = 0; i < iterations; i++) {
            additionalVar += i;
            for (int k = 0; k < iterations; k++) {
                additionalVar += k;
                testVar += 5;
            }
        }
        return testVar + additionalVar;
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        res += testLoopAddInt();
        try {
            throw new Exception("My exception"); 
        } catch (Exception e) {
        } finally {
        }
        return res;
    }

    public static void main(String[] args) {
         System.out.println(new Main().$noinline$testTryCatch());
    }
}  

