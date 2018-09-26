class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        char workJ = '\u0000';

        System.out.println("Initial workJ value is " + String.format("\\u%04x", (int)workJ));

        for(int i = 0; i < iterations; i++) {
            workJ = (char)(test.shim() + workJ);
        }

        System.out.println("Final workJ value is " + String.format("\\u%04x", (int)workJ));
    }
}
