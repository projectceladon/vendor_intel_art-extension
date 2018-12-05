class Main {
    final static int iterations = 10;

    public static String simple_method() {
        String string;
        string = "This is a string";
        return string;
    }

    public static void main(String[] args) {
        String workJ = " ";

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
