// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test extends SuperTest {

    Test(int iterations) {
        super(iterations);
    }

    public double getThingies(double[] arr, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        return super.getThingies(arr, i, d1, d2, d3, d4, d5, d6);
    }

    public void setThingies(double[] arr, double newThingy, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        super.setThingies(arr, newThingy, i, d1, d2, d3, d4, d5, d6);
    }
}

