// InvokeDirect:

class Test
{
    private double b(int div) {
        return 1.0d/div;
    }
    public void a(int iter) {
        double c = 0.0d;
        for (int i = 1; i < iter; i++) {
            c = b(i);
            System.out.println(c);
        }
        System.out.println(c);
   }

}


