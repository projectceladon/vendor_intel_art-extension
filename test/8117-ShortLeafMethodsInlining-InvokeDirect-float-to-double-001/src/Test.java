class Test {
    public double shim(float jj){
        return simple_method(jj);
    }

    private double simple_method(float jj) {
        double ii;
        ii = (double)jj;
        return ii;
    }

}
