// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public byte getThingies(byte[] arr, int i) {
        return super.getThingies(arr, i);
    }

    public void setThingies(byte[] arr, byte newThingy, int i) {
        super.setThingies(arr, newThingy, i);
    }
}

