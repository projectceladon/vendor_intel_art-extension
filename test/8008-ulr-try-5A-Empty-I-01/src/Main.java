
public class Main {

// after 
    public int loop() {
        int a=6;
        try {throw new Exception();}
        catch (Exception e){ a++;   }
        finally {a-=2; }
        for (int i = 0; i < 10000; i++) {  ;}; 
        return a;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
