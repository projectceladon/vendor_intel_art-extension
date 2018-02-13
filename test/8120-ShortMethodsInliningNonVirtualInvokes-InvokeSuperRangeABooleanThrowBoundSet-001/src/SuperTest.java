// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static boolean[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new boolean[iterations];
    }

    public boolean getThingies(boolean[] arr, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        return arr[i];
    }

    public void setThingies(boolean[] arr, boolean newThingy, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        arr[i] = newThingy;
    }
}

