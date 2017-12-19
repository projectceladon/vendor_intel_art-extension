
/**
 *
 * Expected result: no sinking because of sinking candidate is inside switch: 
 * breaking limitation: "instruction must be executed per iteration "
 *
 **/

public class Main {                                                                                                                                                   
    final int iterations = 100;

    public int testLoopAddInt() {
        int testVar = 10;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar += setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar += setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar += setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public int testLoopSubInt() {
        int testVar = 10;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar -= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar -= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar -= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public long testLoopSubLong() {
        long testVar = 10;
        int additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar -= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar -= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar -= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public int testLoopMulInt() {
        int testVar = 1;
        int additionalVar = 0;
        int setToConstantVar = 6;
        for (int i = -8; i < 2; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar *= setToConstantVar;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar *= setToConstantVar;
		  additionalVar--;
		default:
                  testVar *= setToConstantVar;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public long testLoopMulLong() {
        long testVar = 1;
        int additionalVar = 0;
        long setToConstantVar = 6L;
        for (long i = -10; i < 2; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar *= setToConstantVar;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar *= setToConstantVar;
		  additionalVar--;
		default:
                  testVar *= setToConstantVar;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public int testLoopDivInt() {
        int testVar = 10;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar /= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar /= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar /= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public long testLoopDivLong() {
        long testVar = 10;
        int additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar /= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar /= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar /= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public int testLoopRemInt() {
        int testVar = 10;
        int additionalVar = 0;
        int setToConstantVar = 3;
        for (int i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar %= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar %= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar %= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public long testLoopRemLong() {
        long testVar = 10;
        int additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar %= setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar %= setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar %= setToConstantVar + 3;
		  additionalVar++;
		  break; 
	      }
        }
        return testVar + additionalVar;
    }

    public long testLoopAddLong() {
        long testVar = 10;
        int additionalVar = 0;
        long setToConstantVar = 3;
        for (long i = -10; i < iterations; i++) {
              additionalVar += (i*2)%5;
	      switch (additionalVar) {
		case 0:
                  testVar += setToConstantVar + 1;
		  additionalVar = additionalVar + 2 ;
		  break;
		case 10:
	          testVar += setToConstantVar + 2;
		  additionalVar--;
		default:
                  testVar += setToConstantVar + 3;
		  additionalVar++;
		  break; 
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
System.out.println(new Main().testLoopMulLong());
System.out.println(new Main().testLoopMulInt());
System.out.println(new Main().testLoopSubLong());
System.out.println(new Main().testLoopSubInt());
    }

}  

