class Test {

    private double simple_method(double jj, double ii) {
        jj--;
        try {
            throw new Exception("Test");
        } catch (Exception e) {
            jj++;
        }
        jj = ii + jj;
        return jj;
    }
    public double shim(double jj, double ii) {
        return simple_method(jj, ii);
    }
}
