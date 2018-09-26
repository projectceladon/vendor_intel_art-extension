class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        int nextThingy = -10;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.getThingies() + i;
            test.setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
    }
}
