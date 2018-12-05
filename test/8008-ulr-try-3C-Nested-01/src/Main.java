
public class Main {

    // Test not a simple loop: nested
    // M: inner loops are now supported

    public int loop() {
        int sum = 0;
        try { throw new Exception();}
        catch (Exception e)
        {
        for (int i = 0; i < 200; i++) {
            for (int j = 0; j < 200; j++) {
                sum += 5;
            }
        }
        sum++;
        }
        finally { sum-=2; }

        return sum;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
