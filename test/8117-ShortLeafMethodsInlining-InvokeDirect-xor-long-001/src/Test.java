class Test {
    public long shim(long jj, long kk){
        return simple_method(jj, kk);
    }

    private long simple_method(long jj, long kk) {
        jj = kk ^ jj;
        return jj;
    }

}
