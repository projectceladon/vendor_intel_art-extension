
public class Main {

    // Test a loop that contains return

    public int loop() {
        for (int i = 0; i < 10000; i++) {
            return i + 1;
        }
        return 0;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
