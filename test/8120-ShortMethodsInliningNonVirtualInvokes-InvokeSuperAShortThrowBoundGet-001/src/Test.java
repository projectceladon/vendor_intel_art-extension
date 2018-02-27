// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public short getThingies(short[] arr, int i) {
        return super.getThingies(arr, i);
    }

    public void setThingies(short[] arr, short newThingy, int i) {
        super.setThingies(arr, newThingy, i);
    }
}

