class Test {
    public double shim(double jj, double kk){
        return simple_method(jj, kk);
    }

    private double simple_method(double jj, double kk) {
        jj = kk % jj;
        jj = jj % kk;
        return jj;
    }

}
