class Main {
    final static int iterations = 10;

    public static int simple_method(int jj, int ii) {
        jj = ii + jj;
        return jj;
    }

    public static void main(String[] args) {
        int workJ = 10;
        int workK = 15;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
