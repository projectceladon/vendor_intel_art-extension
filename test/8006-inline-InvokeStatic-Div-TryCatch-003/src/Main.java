// InvokeStatic

public class Main
{
    public static float b(int div) {
        return 1.0f / div;
    }

    public static int d(int div) {
        return 1 / div;
    }

    public void test() {
        float c = 0.0f;
        for (int i = 1; i < 11; i++) {
            try {
                c = (float)d(0);
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
