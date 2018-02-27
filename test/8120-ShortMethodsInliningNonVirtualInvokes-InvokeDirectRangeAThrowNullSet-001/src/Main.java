// The test checks that stack after NullPointerException occurs is correct despite inlining
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
        int nextThingy = -10;
        char c1 = 'a';
        char c2 = 'b';
        char c3 = 'c';
        char c4 = 'd';
        char c5 = 'e';
        char c6 = 'f';

        int sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = sumArrElements + test.thingiesArray[i];
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.gimme(test.thingiesArray, i, c1, c2, c3, c4, c5, c6) - i*1;
            test.hereyouare(test.thingiesArray, nextThingy, i, c1, c2, c3, c4, c5, c6);
        }

    }
}
