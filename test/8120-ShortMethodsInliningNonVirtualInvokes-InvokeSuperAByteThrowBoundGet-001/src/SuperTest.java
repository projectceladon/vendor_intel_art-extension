// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static byte[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new byte[iterations];
    }

    public byte getThingies(byte[] arr, int i) {
        return arr[i];
    }

    public void setThingies(byte[] arr, byte newThingy, int i) {
        arr[i] = newThingy;
    }
}

