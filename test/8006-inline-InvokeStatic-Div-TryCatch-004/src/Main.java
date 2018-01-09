// InvokeStatic

public class Main
{
    public static double b(int div) {
        return 1.0d / div;
    }

    public static int d(int div) {
        return 1 / div;
    }

    public void test() {
        double c = 0.0d;
        for (int i = 1; i < 11; i++) {
            try {
                c = (double)d(0);
            } catch (Exception e) {
                // ignore
            } finally {
                c = b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
    }

    public static void main(String[] args)
    {
        new Main().test();
    }
}
