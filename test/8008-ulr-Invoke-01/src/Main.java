
public class Main {

    // Test a loop that has invokes
    public int loop() {

        int sum = 0;
        for (int i = 0; i < 10000; i++) {
            sum = (int) Math.pow(i, 2);
        }
        return sum;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
