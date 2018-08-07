class Test {

    private double simple_method(double jj, double ii) {
        jj = ii + jj;
        return jj;
    }
    public double shim(double jj, double ii){
        double j = -1.0;
        try {
            throw new Exception("Test");
        } catch (Exception e) {
            j += 1.0;
            j += simple_method(jj, ii);
        }
        return j;
    }
}
