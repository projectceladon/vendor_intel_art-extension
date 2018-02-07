class Main {
    final static int iterations = 10;

    public static float simple_method(float jj) {
        return (-jj);
    }

    public static void main(String[] args) {
        float workJ = 3.4028235e38f;
        float workK = 0.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
