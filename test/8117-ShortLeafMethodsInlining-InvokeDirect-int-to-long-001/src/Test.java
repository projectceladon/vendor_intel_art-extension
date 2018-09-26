class Test {
    public long shim(int jj){
        return simple_method(jj);
    }

    private long simple_method(int jj) {
        long ii;
        ii = (long)jj;
        return ii;
    }

}
