// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class SuperTest {
    static float[] thingiesArray;

    SuperTest(int iterations) {
        this.thingiesArray = new float[iterations];
    }

    public float getThingies(float[] arr, int i) {
        return arr[i];
    }

    public void setThingies(float[] arr, float newThingy, int i) {
        arr[i] = newThingy;
    }
}

