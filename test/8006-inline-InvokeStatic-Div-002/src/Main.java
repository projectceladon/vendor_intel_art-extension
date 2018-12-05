// InvokeStatic

public class Main
{
    public static double b(int div) {
        return 1.0d/div;
    }

    public void test() {
        double c = 0.0d;
        for (int i = 1; i < 11; i++) {
            c = b(i);
            System.out.println(c);
        }
        System.out.println(c);
    }

    public static void main(String[] args)
    {
        new Main().test();
    }
}


