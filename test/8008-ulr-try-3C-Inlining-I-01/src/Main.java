//disabled
public class Main {

    // Test a loop with expected inlining

    public static int getValue() {
        return 5;
    }

    public static int loop() {
        int res = 0;
        try { throw new Exception();}
        catch (Exception e){ res++; 
            for (int i = 0; i < 10000; i++) {
                res += getValue() * 2;
            }
        }
        finally { res-=2; }

        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
