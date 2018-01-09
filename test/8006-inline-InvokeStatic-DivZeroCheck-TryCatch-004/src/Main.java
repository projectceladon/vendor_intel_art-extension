// InvokeStatic

public class Main
{
    public static long b(long div) {
        if (div == 10L) {
            div = 0L;
        }
        long d = 1L / div;
        return d;
    }

    public static int d(int div) {
        return 1 / div;
    }

    public void test() {
        long c = 0L;
        for (int i = 1; i < 11; i++) {
            try {
                c = (long)d(0);
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
