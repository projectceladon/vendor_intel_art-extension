
public class Main {
// catch
    public int loop() {
        int a=6;
        try {throw new Exception();}
        catch (Exception e){ a++;  for (int i = 0; i < 10000; i++) {  ;} }
        finally { a-=2; }
        return a;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
