class Test {

    public double shim(double jj, double ii){
        return simple_method(jj, ii);
    }
    private double simple_method(double jj, double ii) {
        jj = ii + jj;
        jj = ii + jj;
        return jj;
    }

}
