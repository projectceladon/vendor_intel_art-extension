class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        long workJ = 0xFFEFAF23;
        float workK = 0.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.shim(workJ) + i;
            workJ = (long)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
