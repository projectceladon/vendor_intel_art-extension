// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        byte nextThingy = -128;
        byte sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (byte)(nextThingy + 1);
            sumArrElements = (byte)(sumArrElements + SuperTest.thingiesArray[i]);
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = (byte)(test.getThingies(SuperTest.thingiesArray, i) + 1);
            if (i == iterations - 1)
                SuperTest.thingiesArray = null;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
        }

    }
}
