// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        Foo nextThingy = new Foo();
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = new Foo();
        }

        for(int i = 0; i < iterations + 1; i++) {
            nextThingy = test.getThingies(SuperTest.thingiesArray, i);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
        }

    }
}
