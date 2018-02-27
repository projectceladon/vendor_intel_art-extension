

public class Main extends TestUtils {

    public int test1(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 2000f; i > 0; i--) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test2(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 0.05f; i > -1111.5f; i -= 0.5f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test3(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 1000.25f; i > -1000.25f; i -= 0.25f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test4(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 2000.25f; i > 125.5f; i -= 0.125f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test5(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 1010.11f; i > 10.1f; i -= 0.1f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test6(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 11000.1f; i > 10000.11f; i -= 0.01f) {
            f *= 1.0001f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test7(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 0.01f; i > -1000.111f; i -= 0.001f) {
            f *= 1.000003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test8(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 2425.5f; i > 21f; i -= 10.5f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test9(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 1126.125f; i > -1126.125f; i -= 125.125f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public static void main(String[] args) {
        TestUtils.runTests(new Main(), 1.00001f);
    }
}
