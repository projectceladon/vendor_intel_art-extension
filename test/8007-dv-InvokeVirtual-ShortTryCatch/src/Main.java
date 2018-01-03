import java.util.*;

public class Main {

    public int div(int d) {
        return 1 / d;
    }

    public void runTestNewInstance() {
        try {
            div(0);
        } catch (Exception e) {
            ArrayList<Short> list = new ArrayList<Short>();
            list.add((short)33);
        }
    }

    static public void main(String[] args) {
       Main n = new Main();
    }
}

