// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public Foo getThingies(Foo[] arr, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return super.getThingies(arr, i, f1, f2, f3, f4, f5, f6);
    }

    public void setThingies(Foo[] arr, Foo newThingy, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        super.setThingies(arr, newThingy, i, f1, f2, f3, f4, f5, f6);
    }
}

