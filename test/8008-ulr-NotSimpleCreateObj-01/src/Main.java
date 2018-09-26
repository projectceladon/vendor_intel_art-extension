
public class Main {

    // Test a loop that creates a new object

    public int loop() {
        for (int i = 0; i < 10000; i++) {
            Object obj = new Object();
        }
        return 0;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
