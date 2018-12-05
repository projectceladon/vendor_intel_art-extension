// InvokeDirect:

class Test
{
    private double b(int div) {
        return 1.0d / div;
    }

    private int d(int div) {
        return 1 / div;
    }

    public void a(int iter) {
        double c = 0.0d;
        for (int i = 1; i < iter; i++) {
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
}
