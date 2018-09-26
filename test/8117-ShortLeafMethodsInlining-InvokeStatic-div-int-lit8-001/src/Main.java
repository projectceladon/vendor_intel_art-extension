class Main {
    final static int iterations = 10;

    public static int simple_method(int jj) {
        jj = jj / 11;
        return jj;
    }

    public static void main(String[] args) {
        int workJ = 1123456789;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
