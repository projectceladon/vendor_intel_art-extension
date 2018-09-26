
public class Main {

    // Test a loop that throws
    public int loop() {
    
        String strnull = null;
        String str = new String();
        for (int i = 0; i < 10000; i++) {
            str = strnull.toString();
        }
        return 0;
    }

    public static void main(String[] args) {

        int res = 0;
        try {
            res = new Main().loop();
        } catch (Exception e) {
            System.out.println(e.getClass());
        }
        System.out.println(res);
    }
}
