// The test checks that stack after NullPointerException occurs is correct despite inlining
class SuperTest {
    static int[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new int[iterations];
    }

    public int getThingies(int[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return arr[i];
    }

    public void setThingies(int[] arr, int newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        arr[i] = newThingy;
    }
}

