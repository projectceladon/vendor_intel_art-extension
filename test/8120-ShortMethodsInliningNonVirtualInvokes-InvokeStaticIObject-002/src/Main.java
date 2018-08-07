class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        Foo nextThingy = new Foo('F');
        Foo tmpThingy;

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Initial nextThingy.a value is " + nextThingy.a);

        for(int i = 0; i < iterations; i++) {
            nextThingy = Test.getThingies(test);
            tmpThingy = nextThingy.toFoo();
            Test.setThingies(test, tmpThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Final nextThingy.a value is " + nextThingy.a);

    }
}
