
/**
*
* Simplest case when optimization should be applied
*  1 sinking expected
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    final int testLoopRemInt() {
        int testVar = 100;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
              testVar %= setToConstantVar;
              additionalVar += (i*2)%5;
	      if (additionalVar > 10) {
		  additionalVar++;
	      }
        }
        testVar = testVar + additionalVar;
        return testVar;
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        try {
            throw new Exception("My exception"); 
        } catch (Exception e) {
        } finally {
        }
        res += testLoopRemInt();
        return res;
    }

    public static void main(String[] args) {
         System.out.println(new Main().$noinline$testTryCatch());
    }
}  

