class Main {
    final static int iterations = 10;

    public static char simple_method(int jj) {
        char ii;
        ii = (char)jj;
        return ii;
    }

    public static void main(String[] args) {
        int workJ = 123456789;
        char workK = 'a';

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = (char)(simple_method(workJ) + i);
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
