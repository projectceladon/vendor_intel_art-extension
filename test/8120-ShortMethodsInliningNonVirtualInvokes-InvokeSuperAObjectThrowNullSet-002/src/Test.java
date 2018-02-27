// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public Foo getThingies(Foo[] arr, int i) {
        return super.getThingies(arr, i);
    }

    public void setThingies(Foo[] arr, Foo newThingy, int i) {
        super.setThingies(arr, newThingy, i);
    }
}

