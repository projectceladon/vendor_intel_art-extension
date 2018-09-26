// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static boolean[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new boolean[iterations];
    }

    public boolean getThingies(boolean[] arr, int i) {
        return arr[i];
    }

    public void setThingies(boolean[] arr, boolean newThingy, int i) {
        arr[i] = newThingy;
    }
}

