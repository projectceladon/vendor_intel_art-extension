class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 123456789;
        long workK = 0l;

        System.out.println("Initial workJ value is " + workJ);

        for(long i = 0l; i < iterations; i++) {
            workK = test.shim(workJ) + i;
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
