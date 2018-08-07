// The test checks that stack after NullPointerException occurs is correct despite inlining
class SuperTest {
    static char[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new char[iterations];
    }

    public char getThingies(char[] arr, int i) {
        return arr[i];
    }

    public void setThingies(char[] arr, char newThingy, int i) {
        arr[i] = newThingy;
    }
}

