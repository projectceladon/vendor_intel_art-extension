class Test {
    public long shim(float jj){
        return simple_method(jj);
    }

    private long simple_method(float jj) {
        long ii;
        ii = (long)jj;
        return ii;
    }

}
