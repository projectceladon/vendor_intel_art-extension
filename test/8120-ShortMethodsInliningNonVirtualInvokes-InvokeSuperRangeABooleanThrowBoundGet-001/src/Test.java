// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public boolean getThingies(boolean[] arr, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        return super.getThingies(arr, i, i1, i2, i3, i4, i5, i6);
    }

    public void setThingies(boolean[] arr, boolean newThingy, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        super.setThingies(arr, newThingy, i, i1, i2, i3, i4, i5, i6);
    }
}

