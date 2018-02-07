class Main {
    final static int iterations = 10;

    public static float simple_method(float jj, float ii) {
        jj = ii + jj;
        jj = ii + jj;
        return jj;
    }

    public static void main(String[] args) {
        float workJ = 10.0f;
        float workK = 15.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(float i = 0.0f; i < iterations; i++) {
            workJ = simple_method(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
