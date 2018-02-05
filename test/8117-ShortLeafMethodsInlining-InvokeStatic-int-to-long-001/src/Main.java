class Main {
    final static int iterations = 10;

    public static long simple_method(int jj) {
        long ii;
        ii = (long)jj;
        return ii;
    }

    public static void main(String[] args) {
        int workJ = 123456789;
        long workK = 0l;

        System.out.println("Initial workJ value is " + workJ);

        for(long i = 0l; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
