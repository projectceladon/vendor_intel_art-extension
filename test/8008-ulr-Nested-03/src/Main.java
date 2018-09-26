
public class Main {

    // Test not a simple loop: nested
    // M: inner loops are now supported

    public int loop() {
    
        int sum = 0;
        int used1 = 10;
        for (int i = 0; i < 200; i++) {
            for (int j = 0; j < 1200; j++) {
                sum += 5;
                used1 -= 3;
            } 
            for (int j = 0; j < 1200; j++) {
                sum += 5;
            } 
            sum += 17;
            used1 -= 19;


        }
        return sum + used1;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
