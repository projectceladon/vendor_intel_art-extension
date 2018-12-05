// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public short getThingies(short[] arr, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        return super.getThingies(arr, i, s1, s2, s3, s4, s5, s6);
    }

    public void setThingies(short[] arr, short newThingy, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        super.setThingies(arr, newThingy, i, s1, s2, s3, s4, s5, s6);
    }
}

