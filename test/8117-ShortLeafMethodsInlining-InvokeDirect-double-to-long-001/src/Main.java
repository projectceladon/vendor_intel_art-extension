class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 4.9e-324;
        long workK = 0x0L;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.shim(workJ) + i;
            workJ = (double)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
