// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        double nextThingy = -10.0;
        double sumArrElements = 0.0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = i;
            sumArrElements = sumArrElements + SuperTest.thingiesArray[i];
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.getThingies(SuperTest.thingiesArray, i) - i*1.0;
            if (i == iterations - 1)
                SuperTest.thingiesArray = null;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
        }

    }
}
