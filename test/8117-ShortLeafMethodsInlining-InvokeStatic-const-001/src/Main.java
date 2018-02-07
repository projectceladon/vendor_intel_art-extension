class Main {
    final static int iterations = 10;

    public static char simple_method() {
        char    l2;
        l2 = '\uffff';
        return l2;
    }

    public static void main(String[] args) {
        char workJ = '\u0000';

        System.out.println("Initial workJ value is " + String.format("\\u%04x", (int)workJ));

        for(int i = 0; i < iterations; i++) {
            workJ = (char)(simple_method() + workJ);
        }

        System.out.println("Final workJ value is " + String.format("\\u%04x", (int)workJ));
    }
}
