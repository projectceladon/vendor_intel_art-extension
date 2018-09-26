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
        short nextThingy = -32768;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        short sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = (short) (sumArrElements + test.thingiesArray[i]);
        }

        try {
            for(int i = 0; i < iterations + 1; i++) {
                if (i < iterations)
                   nextThingy = (short) (test.gimme(test.thingiesArray, i, t1, t2, t3, t4, t5, t6) + 1);
                test.hereyouare(test.thingiesArray, nextThingy, i, t1, t2, t3, t4, t5, t6);
            }
        } catch (ArrayIndexOutOfBoundsException ex) {
            ex.printStackTrace();
        } catch (Exception ex) {
            System.out.println("Caught unhandled exception");
            ex.printStackTrace();
        }

    }
}
