class Test {
    float thingies = 0;

    private float getThingies() {
        return thingies;
    }

    private void setThingies(float newThingies) {
        thingies = newThingies;
    }

    public float gimme() {
       return getThingies();
    }

    public void hereyouare(float newThingies) {
        setThingies(newThingies);
    }
}

