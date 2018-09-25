
/**
*
* Handling multiple loops: should be supported
* Expected result: 4 sinkings
*
**/

public class Main {                                                                                                                                                   
    final int iterations = 1100;

    public int testLoopAddInt() {
        int testVar1 = 10;
        int testVar2 = 20;
        int testVar3 = 30;
        int testVar4 = 40;
        long additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
              testVar1 += 1;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar2 += 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar3 += 3;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar4 += 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public long testLoopAddLong() {
        long testVar1 = 10;
        long testVar2 = 20;
        long testVar3 = 30;
        long testVar4 = 40;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
              testVar1 += 1;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar2 += 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar3 += 3;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar4 += 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public int testLoopSubInt() {
        int testVar1 = 10;
        int testVar2 = 20;
        int testVar3 = 30;
        int testVar4 = 40;
        long additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
              testVar1 -= 1;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar2 -= 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar3 -= 3;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar4 -= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public long testLoopSubLong() {
        long testVar1 = 10;
        long testVar2 = 20;
        long testVar3 = 30;
        long testVar4 = 40;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
              testVar1 -= 1;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar2 -= 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar3 -= 3;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar4 -= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public int testLoopMulInt(int n) {
        int testVar1 = 1;
        int testVar2 = 1;
        int testVar3 = 2;
        int testVar4 = 2;
        long additionalVar = 0;
        for (int i = 0; i < 11; i++) {
              testVar1 *= 6;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%3 + i%4 + n%2 + n%3 + n%4;
        }
        for (int i = 0; i < 11; i++) {
              testVar2 *= 6;
              additionalVar = ((i)*2)%5 + n + n*i + i%4 + i%5 + i%6 + n%2 + n%3 + n%4;
        }
        for (int i = 0; i < 11; i++) {
              testVar3 *= 6;
              additionalVar = ((i)*2)%5 + n + n*i + i%7 + i%8 + i%9 + n%2 + n%3 + n%4;
        }
        for (int i = 0; i < 11; i++) {
              testVar4 *= 6;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%5 + i%8 + n%2 + n%3 + n%4;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public long testLoopMulLong(long n) {
        long testVar1 = 1;
        long testVar2 = 1;
        long testVar3 = 2;
        long testVar4 = 2;
        long additionalVar = 0;
        for (long i = 0; i < 12; i++) {
              testVar1 *= 6L;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%5 + i%8 + n%2 + n%3 + n%4;
        }
        for (long i = 0; i < 12; i++) {
              testVar2 *= 6L;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%5 + i%8 + n%2 + n%3 + n%4;
        }
        for (long i = 0; i < 12; i++) {
              testVar3 *= 6L;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%5 + i%8 + n%2 + n%3 + n%4;
        }
        for (long i = 0; i < 12; i++) {
              testVar4 *= 6L;
              additionalVar = ((i)*2)%5 + n + n*i + i%2 + i%5 + i%8 + n%2 + n%3 + n%4;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public int testLoopDivInt() {
        int testVar1 = 1000;
        int testVar2 = 2000;
        int testVar3 = 3000;
        int testVar4 = 4000;
        long additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
              testVar1 /= 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar2 /= 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar3 /= 3;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar4 /= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public long testLoopDivLong() {
        long testVar1 = 1000;
        long testVar2 = 2000;
        long testVar3 = 3000;
        long testVar4 = 4000;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
              testVar1 /= 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar2 /= 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar3 /= 3;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar4 /= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public int testLoopRemInt() {
        int testVar1 = 1000;
        int testVar2 = 2000;
        int testVar3 = 3000;
        int testVar4 = 4000;
        long additionalVar = 0;
        for (int i = 0; i < iterations; i++) {
              testVar1 %= 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar2 %= 2;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar3 %= 3;
              additionalVar = ((i)*2)%5;
        }
        for (int i = 0; i < iterations; i++) {
              testVar4 %= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public long testLoopRemLong() {
        long testVar1 = 1000;
        long testVar2 = 2000;
        long testVar3 = 3000;
        long testVar4 = 4000;
        long additionalVar = 0;
        for (long i = 0; i < iterations; i++) {
              testVar1 %= 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar2 %= 2;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar3 %= 3;
              additionalVar = ((i)*2)%5;
        }
        for (long i = 0; i < iterations; i++) {
              testVar4 %= 4;
              additionalVar = ((i)*2)%5;
        }
        testVar1 += additionalVar;
        testVar2 += additionalVar;
        testVar3 += additionalVar;
        testVar4 += additionalVar;
        return testVar1 + testVar2 + testVar3 + testVar4;
    }

    public static void main(String[] args)
    {
        System.out.println(new Main().testLoopAddInt());
        System.out.println(new Main().testLoopAddLong());
        System.out.println(new Main().testLoopRemLong());
        System.out.println(new Main().testLoopRemInt());
        System.out.println(new Main().testLoopDivLong());
        System.out.println(new Main().testLoopDivInt());
        System.out.println(new Main().testLoopMulLong(10));
        System.out.println(new Main().testLoopMulInt(10));
        System.out.println(new Main().testLoopSubLong());
        System.out.println(new Main().testLoopSubInt());
    }

}  

