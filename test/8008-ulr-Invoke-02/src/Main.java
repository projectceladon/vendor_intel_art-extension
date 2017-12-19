
public class Main {

    class Test {

        int square(int i) {
            return i * i;
        }
    }

    // Test a loop that has invokes
    public int loop() {

        int res = 0;
        Test t = new Test();
        for (int i = 0; i < 10000; i++) {
            res = t.square(i);
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
