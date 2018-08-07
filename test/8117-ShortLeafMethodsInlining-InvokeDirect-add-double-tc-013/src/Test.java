class Test {

    private double simple_method(double jj, double ii) {
        jj = ii + jj;
        return jj;
    }
    public double shim(double jj, double ii) throws Exception {
        throw new Exception("Test");
    }
    public double sh1m(double jj, double ii) {
        return simple_method(jj, ii);
    }
}
