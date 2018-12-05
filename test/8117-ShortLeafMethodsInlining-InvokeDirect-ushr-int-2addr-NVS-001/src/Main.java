class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 0xFFEF;
        int workK = 0xF8E9;

        System.out.format("Initial workJ value is %o%n", workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = test.shim(workJ, workK) + i;
        }

        System.out.format("Final workJ value is %o%n", workJ);
    }
}
