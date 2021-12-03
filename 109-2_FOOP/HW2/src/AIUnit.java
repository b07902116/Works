import tw.waterball.foop.hw2.provided.AI;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class AIUnit extends Unit{
    final AI ai;
    public AIUnit(int troop_index, int unit_index, String[] tokens){
        super(troop_index, unit_index, tokens);
        ai = new AI();
    }
    @Override
    public Action selectAction(Map<String, Action> actionmap){
        List<Integer> valid_list = new ArrayList<>();
        List<String> action_list = getAction_list();
        this.ShowActionList();
        for (int i = 0; i < action_list.size(); i++){
            if (getMP() >= actionmap.get(action_list.get(i)).getMPconsumption()){
                valid_list.add(i);
            }
        }
        int action = ai.selectAction(valid_list);
        return actionmap.get(action_list.get(action));
    }
    @Override
    public List<Integer> selectTargets(Action action, Troop[] troops){
        List<Unit> candidates = action.CandidateList(this, troops);
        List<Integer> targets = null;
        if (action.getTarget_num() > 0) {
            if (candidates.size() > action.getTarget_num()){
                targets = ai.selectTarget(candidates.size(), action.getTarget_num());
            }
        }
        return targets;
    }
}
