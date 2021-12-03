import java.util.List;

import static utils.Inputs.in;

public class FileSystemCLI {
    private Directory root;
    private Directory currentDir;

    public FileSystemCLI(){
        root = new Directory("/", "/");
        root.setParentDir(root);
        currentDir = root;
    }

    public void start() {
        while (true) {
            printCurrentPath();
            String command = in.nextLine();
            if (command.equals("exit")) {
                break;
            }
            executeCommand(command);
        }
    }

    private void printCurrentPath() {
        System.out.println("Current path: " + currentDir.getPath());
    }

    public void executeCommand(String command) {
        String[] segments = command.split("\\s");

        try {
            String commandName = segments[0];
            performCommand(commandName, segments);
        } catch (Exception err) {
            System.err.println("(Debug) Error: " + err.getMessage());
            System.out.println("Illegal command.");
        }
    }

    private void performCommand(String commandName, String[] segments) {
        switch (commandName) {
            case "cd":
                changeDirectory(segments[1]);
                break;
            case "mkdir":
                makeDirectory(segments[1]);
                break;
            case "touch":
                touch(segments[1], segments[2]);
                break;
            case "rm":
                remove(segments[1]);
                break;
            case "cat":
                concatenate(segments[1]);
                break;
            case "ls":
                list();
                break;
            case "search":
                search(segments[1]);
                break;
            case "ln":
                link(segments[1], segments[2]);
                break;
        }
    }

    private void changeDirectory(String directoryName) {
        // TODO
        if (directoryName.equals("..")){
            this.currentDir = currentDir.parentDir;
        }
        else{
            Node node = currentDir.getNode(directoryName);
            if (node instanceof Directory){
                currentDir = (Directory) node;
            }
            else if (node instanceof Link){
                Node dir = currentDir.getNode(((Link) node).getTargetName());
                if (dir == null){
                    System.out.println("Illegal command.");
                }
                else{
                    currentDir = (Directory) dir;
                }
            }
            else{
                System.out.println("Illegal command.");
            }
        }
    }

    private void makeDirectory(String directoryName) {
        // TODO
        if (currentDir.getNode(directoryName) == null){
            String path = (currentDir.equals(root))? "" : currentDir.getPath();
            Directory new_node = new Directory(directoryName, path + "/" + directoryName);
            new_node.setParentDir(currentDir);
            currentDir.addNode(new_node);
        }
        else{
            System.out.println("Illegal command.");
        }

    }

    private void touch(String fileName, String content) {
        // TODO
        if (currentDir.getNode(fileName) == null){
            String path = (currentDir.equals(root))? "" : currentDir.getPath();
            File new_node = new File(fileName, path + "/" + fileName, content);
            currentDir.addNode(new_node);
        }
        else{
            System.out.println("Illegal command.");
        }
    }

    private void remove(String childName) {
        // TODO
        Node node = currentDir.getNode(childName);
        if (node != null){
            currentDir.removeNode(childName);
        }
        else{
            System.out.println("Illegal command.");
        }
    }

    private void concatenate(String fileName) {
        // TODO
        Node node = currentDir.getNode(fileName);
        if (node instanceof File){
            System.out.println(((File) node).getContent());
        }
        else if (node instanceof Link){
            System.out.println(((Link) node).getTargetPath());
        }
        else{
            System.out.println("Illegal command.");
        }
    }

    private void list() {
        // TODO
        List<Node> node_list = currentDir.getNode_list();
        for (Node node : node_list){
            System.out.println(node.getName());
        }
    }

    private void link(String targetName, String linkName) {
        // TODO
        Node targetNode = currentDir.getNode(targetName);
        if (targetNode instanceof Directory){
            String path = (currentDir.equals(root))? "" : currentDir.getPath();
            Link new_node = new Link(linkName, path + "/" + linkName, targetName, path + "/" + targetName);
            currentDir.addNode(new_node);
        }
        else{
            System.out.println("Illegal command.");
        }
    }

    private void search(String keyword) {
        // TODO
        currentDir.preorderSearch(keyword);
    }

}
