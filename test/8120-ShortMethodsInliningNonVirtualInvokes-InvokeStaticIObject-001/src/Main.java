class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        Foo nextThingy = new Foo();

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());

        for(int i = 0; i < iterations; i++) {
            nextThingy = Test.getThingies(test);
            Test.setThingies(test, nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());

    }
}
