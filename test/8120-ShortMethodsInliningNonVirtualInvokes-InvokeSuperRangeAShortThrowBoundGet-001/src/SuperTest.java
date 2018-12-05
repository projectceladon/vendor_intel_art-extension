// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static short[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new short[iterations];
    }

    public short getThingies(short[] arr, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        return arr[i];
    }

    public void setThingies(short[] arr, short newThingy, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        arr[i] = newThingy;
    }
}

