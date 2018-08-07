class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        float workJ = 1.40e-45f;
        double workK = 0.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.simple_method(workJ) + i;
            workJ = (float)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
