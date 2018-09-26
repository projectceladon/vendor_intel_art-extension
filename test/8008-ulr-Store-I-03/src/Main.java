
public class Main {
    // Test a loop that has a store to memory (sput)

    int invar1 = 555;
    static int field = 0;

    public int loop() {
        for (int i = 0; i < 10000; i++) {
            field += invar1;
        }
        return field;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
