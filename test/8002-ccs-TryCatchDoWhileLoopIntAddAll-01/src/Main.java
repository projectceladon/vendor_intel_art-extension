
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    final int testLoopAddInt() {
        int testVar = 1000;

        int i = 0;
        do {
            testVar += 5;
            i++;
        } while (i < iterations);
        return testVar;
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        res += testLoopAddInt();
        try {
            if (testLoopAddInt() > 0) {
                throw new Exception("My exception"); 
            }
        } catch (Exception e) {
            res ^= testLoopAddInt();
        } finally {
            res -= testLoopAddInt();
        }
        res *= testLoopAddInt();
        return res;
    }

    public static void main(String[] args) {
         System.out.println(new Main().$noinline$testTryCatch());
    }
}  

