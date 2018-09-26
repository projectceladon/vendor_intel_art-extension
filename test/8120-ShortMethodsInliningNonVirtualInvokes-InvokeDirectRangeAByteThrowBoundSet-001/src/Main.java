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
        byte c1 = -1;
        byte c2 = -2;
        byte c3 = -3;
        byte c4 = -4;
        byte c5 = -5;
        byte c6 = -6;

        byte sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = (byte) (sumArrElements + test.thingiesArray[i]);
        }
        
        try {
            for(int i = 0; i < iterations + 1; i++) {
                if (i < iterations)
                   nextThingy = (byte) (test.gimme(test.thingiesArray, i, c1, c2, c3, c4, c5, c6) + 1);
                test.hereyouare(test.thingiesArray, nextThingy, i, c1, c2, c3, c4, c5, c6);
            }
        } catch (ArrayIndexOutOfBoundsException ex) {
             ex.printStackTrace();
        } catch (Exception ex) {
            System.out.println("Caught unhandled exception");
            ex.printStackTrace();
        }

    }
}
