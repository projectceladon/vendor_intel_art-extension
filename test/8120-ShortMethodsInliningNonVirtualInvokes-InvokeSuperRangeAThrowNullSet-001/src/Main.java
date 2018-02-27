// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        int nextThingy = -10;
        char c1 = 'a';
        char c2 = 'b';
        char c3 = 'c';
        char c4 = 'd';
        char c5 = 'e';
        char c6 = 'f';

        int sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = i;
            sumArrElements = sumArrElements + SuperTest.thingiesArray[i];
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.getThingies(SuperTest.thingiesArray, i, c1, c2, c3, c4, c5, c6) - i*1;
            if (i == iterations - 1)
                SuperTest.thingiesArray = null;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, c1, c2, c3, c4, c5, c6);
        }
    }
}
