
public class Main {

    public static int loop() {
        int x = 5;
        int y = 7;
        int a = 2;
        int b = 4;
        for (int i = 0; i < 10000; i++) {
            x = a ;
            y = b;
        }
        return x + y ;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
