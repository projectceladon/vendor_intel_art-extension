
public class Main {

// before
    public int loop() {
        int a=6;
        for (int i = 0; i < 10000; i++) {  ;}
        a++;
        try { throw new Exception();}
        catch (Exception e){ a++; }
        finally { a-=2; }
        return a;
    }


    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
