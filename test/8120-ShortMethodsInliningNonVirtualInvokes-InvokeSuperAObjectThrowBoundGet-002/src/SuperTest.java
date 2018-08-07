// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static Foo[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new Foo[iterations];
    }

    public Foo getThingies(Foo[] arr, int i) {
        return arr[i];
    }

    public void setThingies(Foo[] arr, Foo newThingy, int i) {
        arr[i] = newThingy;
    }
}

