// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    double[] thingiesArray;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new double[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = i*1.0;
        }
    }

    private double getThingies(double[] arr, int i) {
        return arr[i];
    }

    private void setThingies(double[] arr, double newThingy, int i) {
        arr[i] = newThingy;
    }

    public double gimme(double[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(double[] arr, double newThingy, int i) {
       if (i == iterr - 1)
           arr = null;
        setThingies(arr, newThingy, i);
    }
}

