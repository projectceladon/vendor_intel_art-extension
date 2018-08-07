// The test checks that stack after NullPointerException occurs is correct despite inlining
class SuperTest {
    static double[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new double[iterations];
    }

    public double getThingies(double[] arr, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        return arr[i];
    }

    public void setThingies(double[] arr, double newThingy, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        arr[i] = newThingy;
    }
}

