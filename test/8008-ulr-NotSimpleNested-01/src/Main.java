
public class Main {

    // Test not a simple loop: nested
    public int loop() {
    
        int sum = 0;
        for (int i = 0; i < 200; i++) {
            for (int j = 0; j < 200; j++) {
                sum += i + j * 2 + 5;
            } 
        }
        return sum;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
