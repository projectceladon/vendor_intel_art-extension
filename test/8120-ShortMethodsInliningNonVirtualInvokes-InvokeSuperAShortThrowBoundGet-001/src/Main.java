// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        short nextThingy = -32768;
        short sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (short)(nextThingy + 1);
            sumArrElements = (short)(sumArrElements + SuperTest.thingiesArray[i]);
        }

        for(int i = 0; i < iterations + 1; i++) {
            nextThingy = (short)(test.getThingies(SuperTest.thingiesArray, i) + 1);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
        }

    }
}
