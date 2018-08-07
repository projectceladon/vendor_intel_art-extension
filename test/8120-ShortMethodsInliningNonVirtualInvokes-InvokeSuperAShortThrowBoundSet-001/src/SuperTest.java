// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static short[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new short[iterations];
    }

    public short getThingies(short[] arr, int i) {
        return arr[i];
    }

    public void setThingies(short[] arr, short newThingy, int i) {
        arr[i] = newThingy;
    }
}

