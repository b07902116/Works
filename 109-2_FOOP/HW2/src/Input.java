import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public final class Input {
    public static Scanner in = new Scanner(System.in);
    public static void InputTroop(Troop troop){
        // #START-TROOP
        in.nextLine();
        String[] tokens = in.nextLine().split(" ");
        while (tokens[0].charAt(0) != '#'){
            troop.AddUnit(tokens);
            tokens = in.nextLine().split(" ");
        }
    }
    public static int InputAction(){
        int action = in.nextInt();
        in.nextLine();
        return action;
    }
    public static List<Integer> InputTargets(){
        List<Integer> targets = new ArrayList<>();
        String[] tokens = in.nextLine().split(", ");
        for (int i = 0; i < tokens.length; i++){
            targets.add(Integer.parseInt(tokens[i]));
        }
        return targets;
    }

}
