// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static byte[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new byte[iterations];
    }

    public byte getThingies(byte[] arr, int i, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        return arr[i];
    }

    public void setThingies(byte[] arr, byte newThingy, int i, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        arr[i] = newThingy;
    }
}

