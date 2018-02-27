// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public boolean getThingies(boolean[] arr, int i) {
        return super.getThingies(arr, i);
    }

    public void setThingies(boolean[] arr, boolean newThingy, int i) {
        super.setThingies(arr, newThingy, i);
    }
}

