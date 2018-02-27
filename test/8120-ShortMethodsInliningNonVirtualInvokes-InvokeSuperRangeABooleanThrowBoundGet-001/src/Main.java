// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        boolean nextThingy = false;
        int j1, j2, j3, j4, j5, j6;
        j1 = 1;
        j2 = 2;
        j3 = 3;
        j4 = 4;
        j5 = 5;
        j6 = 6;

        boolean sumArrElements = false;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = true;
            sumArrElements = sumArrElements & SuperTest.thingiesArray[i];
        }

        for(int i = 0; i < iterations + 1; i++) {
            nextThingy = test.getThingies(SuperTest.thingiesArray, i, j1, j2, j3, j4, j5, j6) || true;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, j1, j2, j3, j4, j5, j6);
        }

    }
}
