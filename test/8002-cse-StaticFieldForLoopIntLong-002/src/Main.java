
/**
 *
 * No sinking expected: according to optimization document only local variable can be sunk
 *
 **/

public class Main {                                                                                                                                                   

    final int iterations = 100;

    public int testLoopAddInt(int n) {
        int additionalVar = 10;
        for (int i = 0; i < iterations; i++) {
            Test.value += 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public int testLoopSubInt(int n) {
        int additionalVar = 10;
        for (int i = 0; i < iterations; i++) {
            Test.value -= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public long testLoopSubLong(long n) {
        long additionalVar = 10;
        for (long i = 0; i < iterations; i++) {
            Test.value -= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public int testLoopMulInt(int n) {
        int additionalVar = 10;
        Test.value = 1;
        for (int i = 0; i < 10; i++) {
            Test.value *= 6;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public long testLoopMulLong(long n) {
        long additionalVar = 10;
        Test.value = 1;
        for (long i = 0; i < 10; i++) {
            Test.value *= 6;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public int testLoopDivInt(int n) {
        int additionalVar = 10;
        for (int i = 0; i < iterations; i++) {
            Test.value /= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public long testLoopDivLong(long n) {
        long additionalVar = 10;
        for (long i = 0; i < iterations; i++) {
            Test.value /= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public int testLoopRemInt(int n) {
        int additionalVar = 10;
        for (int i = 0; i < iterations; i++) {
            Test.value %= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public long testLoopRemLong(long n) {
        long additionalVar = 10;
        for (long i = 0; i < iterations; i++) {
            Test.value %= 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public long testLoopAddLong(long n) {
        long additionalVar = 10;
        for (long i = 0; i < iterations; i++) {
            Test.value += 5;
            additionalVar += i%2 + i%3 + i%4 + i%5 + i%6 + i%7 + i%8 + i%9 + i%10 + 4*(i+5) + 11 + n;
        }

        return Test.value + additionalVar;
    }

    public static void main(String[] args)
    {
         System.out.println(new Main().testLoopAddInt(10));
System.out.println(new Main().testLoopAddLong(10L));
System.out.println(new Main().testLoopRemLong(10L));
System.out.println(new Main().testLoopRemInt(10));
System.out.println(new Main().testLoopDivLong(10L));
System.out.println(new Main().testLoopDivInt(10));
System.out.println(new Main().testLoopMulLong(10L));
System.out.println(new Main().testLoopMulInt(10));
System.out.println(new Main().testLoopSubLong(10L));
System.out.println(new Main().testLoopSubInt(10));
    }

}  

