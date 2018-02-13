// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;

//    public static int getThingies(int i) {
//        return thingiesArray[i];
//    }
//  |[000194] Main.getThingies:(I)I
//  |0000: sget-object v0, LMain;.thingiesArray:[I // field@0001
//  |0002: aget v0, v0, v1
//  |0004: return v0

    public static void main(String[] args) {
        Test test = new Test(iterations);
        byte nextThingy = -128;
        byte sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = (byte) (sumArrElements + test.thingiesArray[i]);
        }

        for(int i = 0; i < iterations +1; i++) {
            nextThingy = (byte) (test.gimme(test.thingiesArray, i) + 1);
            test.hereyouare(test.thingiesArray, nextThingy, i);
        }

    }
}
