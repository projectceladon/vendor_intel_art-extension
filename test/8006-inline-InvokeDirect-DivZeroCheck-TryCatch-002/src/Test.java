// InvokeDirect`:

class Test
{
    private long b(long div) {
       if (div == 10L) {
            div = 0L;
        }
        return 1L / div;
    }

    private int d(int div) {
        return 1 / div;
    }

    public void a(int iter) {
        long c = 0L;
        for (int i = 1; i < iter; i++) {
            try {
                c = (long)d(0);
            } catch (Exception e) {
                c = b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
   }

}
