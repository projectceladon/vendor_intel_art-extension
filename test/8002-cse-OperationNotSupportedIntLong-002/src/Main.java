
/**
  *
  * Expected result: no sinking, operation is not supported
  *
  **/

public class Main {                                                                                                                                                   
    final int iterations = 5;

    public int testLoopInt(int n) {
        int testVar = 2;
        int additionalVar = 0;
        
        for (int i = 0; i < iterations; i++) {
              testVar >>= 2;
              additionalVar += (i*2)%5 + i%3 + i%4 + n;
        }
        testVar += additionalVar;
        return testVar;
    }

    public long testLoopLong(long n) {
        long testVar = 2;
        long additionalVar = 0;
        
        for (long i = 0; i < iterations; i++) {
              testVar >>= 2;
              additionalVar += (i*2)%5 + i%3 + i%4 + n;
        }
        testVar += additionalVar;
        return testVar;
    }


    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopInt(10));
         System.out.println(new Main().testLoopLong(10));
    }

}  

