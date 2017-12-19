
public class Main {

    // Test a loop that throws
    public int loop() {

        int invar1 = 20152015;
        int sum = 0;
        for (int i = 0; i < 10000; i++) {
            sum += invar1 / (i - 5000);
        }
        return sum;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
