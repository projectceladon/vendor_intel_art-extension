
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 *
 * Simplest cases for loops that are already bottom tested
 *
 **/

public class Main {
    final int iterations = 1100;

    /* Positive: Complex Loop is already bottom tested (after gotos)  */
    final int test1(int n) {
        int testVar = 0;
        int i = 0;
        do {
            testVar += 5 + n/2 + i/2;
            i++;
        } while (i < iterations);

        return testVar;
    }

    /* Positive: Complex Loop is already bottom tested (after gotos)  */
    final int test2(int n) {
        int testVar = 0;
        int k = n;
        for ( ; ; ) {
            testVar += 5 + n/2 + k;
            k++;
            if (k > iterations) {
                break;
            }
        }

        return testVar + n + k;
    }


    /* Positive: Complex Loop is already bottom tested (after gotos)  */
    final int test3(int n) {
        int testVar = 0;
        int i = -1;
        while (true) {
            testVar += 5 + n/2 + i/2;
            if (i > iterations || testVar > iterations) {
                break;
            }
        }
        return testVar + n;
    }


    /* Negative: loop shouldn't be marked as bottom tested because of multiple exits  */
    final int test4(int n) {
        int testVar = 0;
        int additionalVar = 1;
        int l = 20;

        int i = 0; 
        do {
            testVar += 5 + n/2 + i/2;
            if (n < l++) {
                break;
            }
            i += 1000000 - 5;
        } while (testVar < iterations);

        return testVar + additionalVar;
    }


    /* Positive: bottom tested count-down loop with float iterator */
    final int test5(int n) {
        int testVar = 0;

        for (float f = 1234.56789f; ; ) {
            f /=123.456789f;
            testVar += f;
            if (f < n ) break;
        }
        return testVar + n;
    }

    final int $noinline$test1() {
        int res = 0;
        int n = 10;

        res += test1(n);

        try {
            if (test1(n) > 0)
                throw new Exception("...");
        } catch (Exception e) {
            res ^= test1(n);
        } finally {
            res -= test1(n);
        }

        res *= test1 (n);

        return res;
    }

    final int $noinline$test2() {
        int res = 0;
        int n = 10;

        res += test2(n);

        try {
            if (test2(n) > 0)
                throw new Exception("...");
        } catch (Exception e) {
            res ^= test2(n);
        } finally {
            res -= test2(n);
        }

        res *= test2 (n);

        return res;
    }

    final int $noinline$test3() {
        int res = 0;
        int n = 10;

        res += test3(n);

        try {
            if (test3(n) > 0)
                throw new Exception("...");
        } catch (Exception e) {
            res ^= test3(n);
        } finally {
            res -= test3(n);
        }

        res *= test3 (n);

        return res;
    }

    final int $noinline$test4() {
        int res = 0;
        int n = 10;

        res += test4(n);

        try {
            if (test4(n) > 0)
                throw new Exception("...");
        } catch (Exception e) {
            res ^= test4(n);
        } finally {
            res -= test4(n);
        }

        res *= test4 (n);

        return res;
    }

    final int $noinline$test5() {
        int res = 0;
        int n = 10;

        res += test5(n);

        try {
            if (test5(n) > 0)
                throw new Exception("...");
        } catch (Exception e) {
            res ^= test5(n);
        } finally {
            res -= test5(n);
        }

        res *= test5 (n);

        return res;
    }

    public void $noinline$runTests() {
        System.out.println($noinline$test1());
        System.out.println($noinline$test2());
        System.out.println($noinline$test3());
        System.out.println($noinline$test4());
        System.out.println($noinline$test5());
    }

    public static void main(String[] args)
    {
        new Main().$noinline$runTests();
    }


}