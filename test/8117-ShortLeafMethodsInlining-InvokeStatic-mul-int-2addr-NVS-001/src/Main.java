class Main {
    final static int iterations = 10;

    public static int simple_method(int jj, int ii) {
        int a= jj;
        a = jj * a;
        a = a * ii;
        return jj;
    }

    public static void main(String[] args) {
        int workJ = 2;
        int workK = 3;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
