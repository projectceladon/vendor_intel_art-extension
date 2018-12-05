// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public byte getThingies(byte[] arr, int i, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        return super.getThingies(arr, i, b1, b2, b3, b4, b5, b6);
    }

    public void setThingies(byte[] arr, byte newThingy, int i, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        super.setThingies(arr, newThingy, i, b1, b2, b3, b4, b5, b6);
    }
}

