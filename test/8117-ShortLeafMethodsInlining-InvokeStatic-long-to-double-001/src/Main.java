class Main {
    final static int iterations = 10;

    public static double simple_method(long jj) {
        double ii;
        ii = (double)jj;
        return ii;
    }

    public static void main(String[] args) {
        long workJ = 0xFFEFAF23;
        double workK = 0.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (long)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
