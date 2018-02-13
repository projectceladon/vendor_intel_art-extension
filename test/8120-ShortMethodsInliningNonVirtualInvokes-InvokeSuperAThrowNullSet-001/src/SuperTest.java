// The test checks that stack after NullPointerException occurs is correct despite inlining
class SuperTest {
    static int[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new int[iterations];
    }

    public int getThingies(int[] arr, int i) {
        return arr[i];
    }

    public void setThingies(int[] arr, int newThingy, int i) {
        arr[i] = newThingy;
    }
}

