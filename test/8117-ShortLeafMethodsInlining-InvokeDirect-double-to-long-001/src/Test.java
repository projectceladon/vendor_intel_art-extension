class Test {
    public long shim(double jj){
        return simple_method(jj);
    }

    private long simple_method(double jj) {
        long ii;
        ii = (long)jj;
        return ii;
    }

}
