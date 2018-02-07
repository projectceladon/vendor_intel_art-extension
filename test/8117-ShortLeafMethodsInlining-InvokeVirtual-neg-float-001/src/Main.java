class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        float workJ = 3.4028235e38f;
        float workK = 0.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
