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
        boolean nextThingy = false;
        boolean sumArrElements = false;
        int j1, j2, j3, j4, j5, j6;
        j1 = 1;
        j2 = 2;
        j3 = 3;
        j4 = 4;
        j5 = 5;
        j6 = 6;

        for(int i = 0; i < iterations; i++) {
            sumArrElements = sumArrElements & test.thingiesArray[i];
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = test.gimme(test.thingiesArray, i, j1, j2, j3, j4, j5, j6) || true;
            test.hereyouare(test.thingiesArray, nextThingy, i, j1, j2, j3, j4, j5, j6);
        }

    }
}
