

public class Main extends TestUtils {

    public int test1(float x) {
        float f = x;
        int res = (int) x;
        for (float i = Float.NaN; i < 10f; i += 0.5f) {
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
        for (float i = -10f; i < Float.NaN; i += 0.25f) {
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
        for (float i = 0; i < 10f; i += Float.NaN) {
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
        for (float i = Float.NaN; i <= 10f; i += 0.5f) {
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
        for (float i = -10f; i <= Float.NaN; i += 0.25f) {
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
        for (float i = 0; i <= 10f; i += Float.NaN) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test7(float x) {
        float f = x;
        int res = (int) x;
        for (float i = Float.NaN; i > -10f; i -= 0.5f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test8(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 10f; i > Float.NaN; i -= 0.25f) {
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
        for (float i = 10; i > 0f; i -= Float.NaN) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test10(float x) {
        float f = x;
        int res = (int) x;
        for (float i = Float.NaN; i > -10f; i -= 0.5f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test11(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 10f; i > Float.NaN; i -= 0.25f) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test12(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 10; i > 0f; i -= Float.NaN) {
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
