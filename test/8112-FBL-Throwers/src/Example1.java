
public class Example1 {


    public static int value  = 50;


    static {
        if (value == 50) {throw new ArithmeticException();}
    }
  }


