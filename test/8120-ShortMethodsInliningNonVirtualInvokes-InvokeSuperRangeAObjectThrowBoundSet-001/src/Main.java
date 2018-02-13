// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        Foo nextThingy = new Foo();
        Foo g1 = new Foo();
        Foo g2 = new Foo();
        Foo g3 = new Foo();
        Foo g4 = new Foo();
        Foo g5 = new Foo();
        Foo g6 = new Foo();

        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = new Foo();
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = test.getThingies(SuperTest.thingiesArray, i, g1, g2, g3, g4, g5, g6);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, g1, g2, g3, g4, g5, g6);
        }

    }
}
