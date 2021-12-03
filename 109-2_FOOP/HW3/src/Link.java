public class Link extends Node{
    String targetName, targetPath;
    public Link(String _name, String _path, String _targetName, String _targetPath){
        this.name = _name;
        this.path = _path;
        this.targetName = _targetName;
        this.targetPath = _targetPath;
    }
    public String getTargetName(){
        return this.targetName;
    }
    public String getTargetPath(){
        return this.targetPath;
    }
}
