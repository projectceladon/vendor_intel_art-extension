class Test {
    double thingies = 0;

    private double getThingies(double d1, double d2, double d3, double d4, double d5, double d6) {
        return thingies;
    }

    private void setThingies(double newThingies, double d1, double d2, double d3, double d4, double d5, double d6) {
        thingies = newThingies;
    }

    public double gimme(double d1, double d2, double d3, double d4, double d5, double d6) {
       return getThingies(d1, d2, d3, d4, d5, d6);
    }

    public void hereyouare(double newThingies, double d1, double d2, double d3, double d4, double d5, double d6) {
        setThingies(newThingies, d1, d2, d3, d4, d5, d6);
    }
}

