// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        short nextThingy = -32768;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        short sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (short)(nextThingy + 1);
            sumArrElements = (short)(sumArrElements + SuperTest.thingiesArray[i]);
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = (short)(test.getThingies(SuperTest.thingiesArray, i, t1, t2, t3, t4, t5, t6) + 1);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, t1, t2, t3, t4, t5, t6);
        }

    }
}
