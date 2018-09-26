class Main {
    final static int iterations = 10;

    public static long simple_method(long jj, long ii) {
        long a = jj;
        a = jj - a;
        a = a - ii;
        return a;
    }

    public static void main(String[] args) {
        long workJ = 256;
        long workK = 1024;

        System.out.println("Initial workJ value is " + workJ);

        for(long i = 0; i < iterations; i++) {
            workJ = simple_method(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
