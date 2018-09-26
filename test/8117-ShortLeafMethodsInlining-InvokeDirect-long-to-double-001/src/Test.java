class Test {
    public double shim(long jj){
        return simple_method(jj);
    }

    private double simple_method(long jj) {
        double ii;
        ii = (double)jj;
        return ii;
    }

}
