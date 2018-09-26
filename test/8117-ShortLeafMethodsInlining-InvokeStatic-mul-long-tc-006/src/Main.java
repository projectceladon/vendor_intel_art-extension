class Main {
    final static int iterations = 10;

    public static long simple_method(long jj, long ii) throws Exception {
        if (ii % 2 == 0)
            throw new Exception("Test");
        jj = ii * jj;
        return jj;
    }

    public static long simple_1method(long jj, long ii) {
        jj = ii * jj;
        return jj;
    }

    public static void main(String[] args) {
        long workJ = 2;
        long workK = 3;

        System.out.println("Initial workJ value is " + workJ);

        for(long i = 0; i < iterations; i++) {
            try {
                workJ = simple_method(workJ, workK) + i;
            } catch (Exception e) {
                workJ = simple_1method(workJ, workK) + i;
            }
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
