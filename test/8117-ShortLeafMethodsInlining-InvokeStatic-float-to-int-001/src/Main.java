class Main {
    final static int iterations = 10;

    public static int simple_method(float jj) {
        int ii;
        ii = (int)jj;
        return ii;
    }

    public static void main(String[] args) {
        float workJ = 1.40e-45f;
        int workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (float)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
