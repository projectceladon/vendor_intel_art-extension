class Main {
    final static int iterations = 10;

    public static float simple_method(int jj) {
        float ii;
        ii = (float)jj;
        return ii;
    }

    public static void main(String[] args) {
        int workJ = 123456789;
        float workK = 0.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(float i = 0.0f; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
