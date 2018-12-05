class Main {
    final static int iterations = 10;

    public static float simple_method() {
        float   l7;
        l7 = 100.0f;
        return l7;
    }

    public static void main(String[] args) {
        float workJ = Float.MIN_VALUE;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
