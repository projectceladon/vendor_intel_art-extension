class Main {
    final static int iterations = 10;

    public static int simple_method(int jj) {
        jj = 100 - jj;
        return jj;
    }

    public static void main(String[] args) {
        int workJ = 256;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
