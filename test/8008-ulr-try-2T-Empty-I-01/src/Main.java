
public class Main {

// try
    public int loop() {
        int a=6;
        try {
        for (int i = 0; i < 10000; i++) {  ;}
        throw new Exception();}
        catch (Exception e){ a++; }
        finally { a-=2; }
        return a;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
