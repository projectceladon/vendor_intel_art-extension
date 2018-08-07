class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        boolean nextThingy = false;
        int j1, j2, j3, j4, j5, j6;
        j1 = 1;
        j2 = 2;
        j3 = 3;
        j4 = 4;
        j5 = 5;
        j6 = 6;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.getThingies(j1, j2, j3, j4, j5, j6) & true;
            test.setThingies(nextThingy, j1, j2, j3, j4, j5, j6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
    }
}
