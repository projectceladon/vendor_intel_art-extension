// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public int getThingies(int[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return super.getThingies(arr, i, c1, c2, c3, c4, c5, c6);
    }

    public void setThingies(int[] arr, int newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        super.setThingies(arr, newThingy, i, c1, c2, c3, c4, c5, c6);
    }
}

