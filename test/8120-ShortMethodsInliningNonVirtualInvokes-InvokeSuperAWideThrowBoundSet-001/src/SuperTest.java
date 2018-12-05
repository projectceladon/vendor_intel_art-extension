// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static double[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new double[iterations];
    }

    public double getThingies(double[] arr, int i) {
        return arr[i];
    }

    public void setThingies(double[] arr, double newThingy, int i) {
        arr[i] = newThingy;
    }
}

