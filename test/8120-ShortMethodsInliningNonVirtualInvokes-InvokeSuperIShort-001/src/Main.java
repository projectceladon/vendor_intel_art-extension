class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        short nextThingy = -32768;

        System.out.println("Initial nextThingy is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short) (test.getThingies() + 1);
            test.setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
        System.out.println("Final test.thingies value is " + test.thingies);

    }
}
