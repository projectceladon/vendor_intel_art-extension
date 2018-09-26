import java.util.*;

public class Main {

    public int div(int d) {
        return 1 / d;
    }

    public void runTestNewInstance() {
        try {
            div(0);
        } catch (Exception e) {
            // ignore
        } finally {
            ArrayList<Long> list = new ArrayList<Long>();
            list.add((long)33);
        }
    }

    static public void main(String[] args) {
       Main n = new Main();
    }
}

