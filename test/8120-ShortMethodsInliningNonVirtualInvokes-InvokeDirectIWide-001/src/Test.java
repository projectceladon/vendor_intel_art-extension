class Test {
    double thingies = 0;

    private double getThingies() {
        return thingies;
    }

    private void setThingies(double newThingies) {
        thingies = newThingies;
    }

    public double gimme() {
       return getThingies();
    }

    public void hereyouare(double newThingies) {
        setThingies(newThingies);
    }
}

