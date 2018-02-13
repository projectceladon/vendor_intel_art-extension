// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    float[] thingiesArray;

    public Test (int iter) {
        this.thingiesArray = new float[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = i*1.0f;
        }
    }

    private float getThingies(float[] arr, int i) {
        return arr[i];
    }

    private void setThingies(float[] arr, float newThingy, int i) {
        arr[i] = newThingy;
    }

    public float gimme(float[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(float[] arr, float newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

