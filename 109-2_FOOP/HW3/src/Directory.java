import java.util.*;

public class Directory extends Node{
    List<Node> node_list;
    Directory parentDir;
    public Directory(String _name, String _path){
        this.name = _name;
        this.path = _path;
        this.node_list = new ArrayList<>();
    }
    public void setParentDir(Directory _parentDir){
        this.parentDir = _parentDir;
    }
    public void addNode(Node new_node){
        node_list.add(new_node);
        node_list.sort(Comparator.comparing(Node::getName));
    }
    public List<Node> getNode_list(){
        return this.node_list;
    }
    public Node getNode(String name){
        Node ret = null;
        for (Node node : this.node_list) {
            if (node.getName().equals(name)) {
                ret = node;
                break;
            }
        }
        return ret;
    }
    public void removeNode(String name){
        Node node = this.getNode(name);
        this.node_list.remove(node);
    }
    public void preorderSearch(String keyword){
        if (this.name.contains(keyword)){
            System.out.println(this.name);
        }
        for (Node node : this.node_list){
            if (node instanceof Directory){
                ((Directory) node).preorderSearch(keyword);
            }
            else{
                if (node.getName().contains(keyword)){
                    System.out.println(node.getName());
                }
            }
        }
    }
}
