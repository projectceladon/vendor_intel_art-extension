import java.util.*;

public class Main {

    public int div(int d) {
        return 1 / d;
    }

    public void runTestNewInstance() {
        try {
            div(0);
        } catch (Exception e) {
            ArrayList<Character> list = new ArrayList<Character>();
            list.add('r');
        }
    }

    static public void main(String[] args) {
       Main n = new Main();
    }
}

