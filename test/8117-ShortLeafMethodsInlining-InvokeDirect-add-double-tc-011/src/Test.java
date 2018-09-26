class Test {

    private double simple_method(double jj, double ii) {
        jj = ii + jj;
        return jj;
    }
    public double shim(double jj, double ii) throws Exception {
        if ((int)(ii) % 2 == 0)
            throw new Exception("Test");
        return simple_method(jj, ii);
    }
    public double sh1m(double jj, double ii) {
        return simple_method(jj, ii);
    }
}
