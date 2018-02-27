// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static char[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new char[iterations];
    }

    public char getThingies(char[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return arr[i];
    }

    public void setThingies(char[] arr, char newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        arr[i] = newThingy;
    }
}

