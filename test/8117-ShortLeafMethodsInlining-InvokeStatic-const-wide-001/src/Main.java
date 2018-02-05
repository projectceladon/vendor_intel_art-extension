class Main {
    final static int iterations = 10;

    public static long simple_method() {
        long    l6;
        l6 = 9223372036854775807l;
        return l6;
    }

    public static void main(String[] args) {
        long workJ = -9223372036854775808l;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
