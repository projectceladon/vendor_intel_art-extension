

public class Main extends TestUtils {

    public int test1(int x) {
        int res = x;
        for (int i = Integer.MAX_VALUE - 10000; i < Integer.MAX_VALUE; i++) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public int test2(int x) {
        int res = x;
        if (res > 0)
            return -1;
        for (int i = Integer.MAX_VALUE - 1000; i <= Integer.MAX_VALUE; i++) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public int test3(int x) {
        int res = x;
        for (int i = Integer.MIN_VALUE + 10000; i > Integer.MIN_VALUE; i--) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public int test4(int x) {
        int res = x;
        if (res > 0)
            return -1;
        for (int i = Integer.MIN_VALUE + 10000; i >= Integer.MIN_VALUE; i--) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public static void main(String[] args) {
        TestUtils.runTests(new Main(), 10);
    }
}
