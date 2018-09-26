

public class Main extends TestUtils {

    // CAR-3170
    public int test1(int x) {
        int i = x;
        for (float f = 5f; f <= 10f; f += 1f) {
            i += 4;
        }
        return i / 4;
    }

    public int test2(int x) {
        int i = x;
        for (float f = 15f; f >= 10f; f -= 1f) {
            i += 4;
        }
        return i / 4;
    }

    public static void main(String[] args) {
        TestUtils.runTests(new Main(), 100);
    }
}
