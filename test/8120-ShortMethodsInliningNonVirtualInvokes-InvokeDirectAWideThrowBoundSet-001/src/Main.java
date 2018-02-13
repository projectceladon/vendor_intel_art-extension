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
        double nextThingy = -10.0;
        double sumArrElements = 0.0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = sumArrElements + test.thingiesArray[i];
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = test.gimme(test.thingiesArray, i) - i*1.0;
            test.hereyouare(test.thingiesArray, nextThingy, i);
        }

    }
}
