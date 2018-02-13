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
        char nextThingy = 'a';
        char sumArrElements = '0';
        char d1 = 'a';
        char d2 = 'b';
        char d3 = 'c';
        char d4 = 'd';
        char d5 = 'e';
        char d6 = 'f';

        for(int i = 0; i < iterations; i++) {
            sumArrElements = (char) (sumArrElements + test.thingiesArray[i]);
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = (char) (test.gimme(test.thingiesArray, i, d1, d2, d3, d4, d5, d6) + 1);
            test.hereyouare(test.thingiesArray, nextThingy, i, d1, d2, d3, d4, d5, d6);
        }

    }
}
