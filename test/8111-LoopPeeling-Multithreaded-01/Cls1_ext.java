
import java.util.*;

public class Cls1_ext extends Cls1 {

    public int i_field4 = 18;

    public static int bar_inlined(int x) {
        return arr_field2[x%10];
    }

    public void set_i_field1(float x) {
        i_field1 = x;
    }

    public void inc_i_field1(float x) {
        for (int i = 0; i < 100000 ; i++) {
            i_field1 += x;
        }
    }
    public void inc_i_field3(double x) {
        for (int i = 0; i < 100000 ; i++) {
            i_field3 += x;
        }
    }

    public void inc_i_field4(int x) {
        for (int i = 0; i < 100000 ; i++) {
            i_field4 += x;
        }
    }


    public double inlined_intrinsic(double d) {
        return Math.sqrt(Math.abs(d));
    }

    public int foo(int x) {
        return x;
    }




}

