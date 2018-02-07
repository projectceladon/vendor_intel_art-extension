class Test {

    private double simple_method(double jj, double ii) {
        jj = ii + jj;
        return jj;
    }

    private double simple_1method(double jj, double ii) throws Exception {
        throw new Exception("Test");
    }

    private double simple_2method(double jj, double ii) throws Exception {
        throw new Exception("Test");
    }

    private double simple_3method(double jj, double ii) throws Exception {
        throw new Exception("Test");
    }

    public double shim(double jj, double ii) throws Exception {
        try {
            return simple_1method(jj, ii);
        } catch (Exception e) {
            return simple_2method(jj, ii);
        } finally {
            return simple_3method(jj, ii);
        }
    }

    public double sh1m(double jj, double ii) {
        return simple_method(jj, ii);
    }
}
