class Main {
    final static int iterations = 10;

    public static long simple_method(double jj) {
        long ii;
        ii = (long)jj;
        return ii;
    }

    public static void main(String[] args) {
        double workJ = 4.9e-324;
        long workK = 0x0L;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (double)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
