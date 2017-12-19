
public class Main {

    // Test not a simple loop: branches
    public int loop() {
    
        int sum = 0;
        for (int i = 0; i < 10000; i++) {
            switch (i - 2500){
                case 0:
                    sum += i - 1;
                    break;
                case 2500:
                    sum += i * 2500;
                    break;
                case 7500:
                    sum += i * 7500;
                    break;
                default:
                    sum += i * 312;
            }
        }
        return sum;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
