

public class Main extends TestUtils {
    public int test1(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 0f; i <= 1000f; i++) {
            f *= 1.003f;
            res <<= 3;
            res += res;
            res += i;
        }
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
        }
        return hash(f, res);
    }

    public int test2(float x) {
        float f = x;
        int res = (int) x;
        try {
            for (float i = 0.05f; i <= 1111.5f; i += 0.5f) {
                f *= 1.003f;
                res <<= 3;
                res += res;
                res += i;
                if (f == res + x)
                    throw new Exception("");
            }
        } catch (Exception e) {
        } finally {
        }
        return hash(f, res);
    }

    public int test3(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
            for (float i = 0.5f; i <= 2000.5f; i += 0.25f) {
                f *= 1.003f;
                res <<= 3;
                res += res;
                res += i;
            }
        } finally {
        }
        return hash(f, res);
    }

    public int test4(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
            for (float i = 0.25f; i <= 2000.5f; i += 0.125f) {
                f *= 1.003f;
                res <<= 3;
                res += res;
                res += i;
            }
        }
        return hash(f, res);
    }

    public int test5(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
        }
        for (float i = 0.1f; i <= 1111.1f; i += 0.1f) {
            f *= 1.03f;
            res <<= 3;
            res += res;
            res += i;
        }
        return hash(f, res);
    }

    public int test6(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
        }
        for (float i = 0.01f; i <= 1111.11f; i += 0.01f) {
            f *= 1.01f;
            res <<= 3;
            res += res;
            res += i;
        }
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
        }
        return hash(f, res);
    }

    public int test7(float x) {
        float f = x;
        int res = (int) x;
        for (float i = 0.001f; i <= 1111.111f; i += 0.001f) {
            f *= 1.03f;
            res <<= 3;
            res += res;
            res += i;
            try {
                throw new Exception("");
            } catch (Exception e) {
            } finally {
            }
        }
        return hash(f, res);
    }

    public int test8(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
        } finally {
            try {
                throw new Exception("");
            } catch (Exception e2) {
                for (float i = 31.5f; i <= 2425.5f; i += 10.5f) {
                    f *= 1.03f;
                    res <<= 3;
                    res += res;
                    res += i;
                }
            } finally {
            }
        }
        return hash(f, res);
    }

    public int test9(float x) {
        float f = x;
        int res = (int) x;
        try {
            throw new Exception("");
        } catch (Exception e) {
            try {
                throw new Exception("");
            } catch (Exception e2) {
            } finally {
                for (float i = 0f; i <= 1126.125f; i += 125.125f) {
                    f *= 1.03f;
                    res <<= 3;
                    res += res;
                    res += i;
                }
            }
        } finally {
        }
        return hash(f, res);
    }

    public static void main(String[] args) {
        TestUtils.runTests(new Main(), 1.00001f);
    }
}
