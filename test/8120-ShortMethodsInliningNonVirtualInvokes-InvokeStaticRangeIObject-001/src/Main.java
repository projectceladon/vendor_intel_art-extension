class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        Foo nextThingy = new Foo();
        Foo g1 = new Foo();
        Foo g2 = new Foo();
        Foo g3 = new Foo();
        Foo g4 = new Foo();
        Foo g5 = new Foo();
        Foo g6 = new Foo();

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());

        for(int i = 0; i < iterations; i++) {
            nextThingy = Test.getThingies(test, g1, g2, g3, g4, g5, g6);
            Test.setThingies(test, nextThingy, g1, g2, g3, g4, g5, g6);
        }

        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());

    }
}
