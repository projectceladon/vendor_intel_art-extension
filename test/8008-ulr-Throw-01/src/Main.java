
public class Main {

    // Test a loop that throws
    public int loop() {
    
        for (int i = 0; i < 10000; i++) {
            throw new RuntimeException("Absolutely new RuntimeException");
        }
        return 0;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
