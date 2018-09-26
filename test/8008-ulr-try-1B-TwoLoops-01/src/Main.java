
public class Main {

    public static int loop() {
        int res = 0;
        for (int i = 0; i < 10000; i++) {
            res += 5;
        }
        for (int j = 0; j < 10000; j++) {
            res -= 3;
        }
        res++;
        try { throw new Exception();}
        catch (Exception e){ res++; }
        finally { res-=2; }

        return --res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
