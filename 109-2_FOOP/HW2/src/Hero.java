import java.util.List;
import java.util.Map;

public class Hero extends Unit{
    public Hero(int troop_index, int unit_index, String[] tokens){
        super(troop_index, unit_index, tokens);
    }
    @Override
    public Action selectAction(Map<String, Action> actionmap){
        List<String> action_list = this.getAction_list();
        this.ShowActionList();
        int action = Input.InputAction();
        while (actionmap.get(action_list.get(action)).getMPconsumption() > getMP()){
            System.out.printf("You can't perform the action: insufficient MP.\n");
            this.ShowActionList();
            action = Input.InputAction();
        }
        return actionmap.get(action_list.get(action));
    }
    @Override
    public List<Integer> selectTargets(Action action, Troop[] troops){
        List<Unit> candidates = action.CandidateList(this, troops);
        List<Integer> targets = null;
        if (action.getTarget_num() > 0) {
            if (candidates.size() > action.getTarget_num()){
                System.out.printf("Select %d targets:", action.getTarget_num());
                for (int i = 0; i < candidates.size(); i++){
                    System.out.printf(" (%d) %s", i, candidates.get(i).getName());
                }
                System.out.println();

                targets = Input.InputTargets();
            }
        }
        return targets;
    }
}
