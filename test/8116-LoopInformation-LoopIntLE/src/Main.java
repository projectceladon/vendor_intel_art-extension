

public class Main extends TestUtils {

    public int test1(int x) {
        int res = x;
        for (int i = 0; i <= x; i++) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public int test2(int x) {
        int res = x;
        for (int i = 0; i <= 0; i++) {
            res <<= 4;
            res += res;
            res += i;
        }
        return res;
    }

    public int test3(int x) {
        int res = x;
        for (int i = 0; i <= 1; i++) {
            res <<= 5;
            res += res;
            res += i;
        }
        return res;
    }

    public int test4(int x) {
        int res = x;
        for (int i = 0; i <= 100; i++) {
            res <<= 3;
            res += res;
            res += i;
        }
        return res;
    }

    public int test5(int x) {
        int res = x;
        for (int i = 0; i <= 10000; i++) {
            res <<= 4;
            res += res;
            res += i;
        }
        return res;
    }

    public static void main(String[] args) {
        TestUtils.runTests(new Main(), 10);
    }
}
