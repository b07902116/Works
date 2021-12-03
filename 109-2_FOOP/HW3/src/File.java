public class File extends Node{
    String content;
    public File(String _name, String _path, String _content){
        this.name = _name;
        this.path = _path;
        this.content = _content;
    }
    public String getContent(){
        return this.content;
    }
}
