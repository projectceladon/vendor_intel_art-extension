class Test {
    public short shim(int jj){
        return simple_method(jj);
    }

    private short simple_method(int jj) {
        short ii;
        ii = (short)jj;
        return ii;
    }

}
