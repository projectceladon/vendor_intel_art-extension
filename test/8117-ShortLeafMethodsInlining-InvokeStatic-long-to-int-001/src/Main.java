class Main {
    final static int iterations = 10;

    public static int simple_method(long jj) {
        int ii;
        ii = (int)jj;
        return ii;
    }

    public static void main(String[] args) {
        long workJ = 0xFFEFAF23;
        int workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (long)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
