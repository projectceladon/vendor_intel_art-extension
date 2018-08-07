// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    double[] thingiesArray;

    public Test (int iter) {
        this.thingiesArray = new double[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = i*1.0;
        }
    }

    private double getThingies(double[] arr, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        return arr[i];
    }

    private void setThingies(double[] arr, double newThingy, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        arr[i] = newThingy;
    }

    public double gimme(double[] arr, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
       return getThingies(arr, i, d1, d2, d3, d4, d5, d6);
    }

    public void hereyouare(double[] arr, double newThingy, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        setThingies(arr, newThingy, i, d1, d2, d3, d4, d5, d6);
    }
}

