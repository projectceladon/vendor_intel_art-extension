
public class Main {

    // Test an empty loop
    public int loop() {
    
        for (int i = 0; i < 10000; i++) {
            ;
        }
        return 0;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
