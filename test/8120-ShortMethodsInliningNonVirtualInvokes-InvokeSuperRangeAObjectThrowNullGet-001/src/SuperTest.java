// The test checks that stack after NullPointerException occurs is correct despite inlining
class SuperTest {
    static Foo[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new Foo[iterations];
    }

    public Foo getThingies(Foo[] arr, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return arr[i];
    }

    public void setThingies(Foo[] arr, Foo newThingy, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        arr[i] = newThingy;
    }
}

