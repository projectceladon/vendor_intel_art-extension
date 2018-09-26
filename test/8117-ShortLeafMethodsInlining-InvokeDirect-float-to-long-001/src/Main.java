class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        float workJ = 1.40e-45f;
        long workK = 0x0L;

        System.out.println("Initial workJ value is " + workJ);

        for(long i = 0x0L; i < iterations; i++) {
            workK = test.shim(workJ) + i;
            workJ = (float)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
