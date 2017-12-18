// InvokeStatic

public class Main
{
    public static int b(int div) {
        if (div == 10) {
            div = 0;
        }
        int d = 1/div;
        return d;
    }

    public void test() {
        int c = 0;
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


