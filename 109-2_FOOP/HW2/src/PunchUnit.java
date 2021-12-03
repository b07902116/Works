import tw.waterball.foop.hw2.provided.Target;

import java.util.List;
import java.util.Map;

public class PunchUnit extends Unit implements Target {
    public PunchUnit(int troop_index, int unit_index, String[] tokens){
        super(troop_index, unit_index, tokens);
    }
    @Override
    public void DeathTask(Troop[] troops){
        // Let the true one to handle
    }
    public void takeOnePunchDamage(int damage){
        setHP(getHP() - damage);
    }
    // selectAction and selectTargets should not be called by PunchUnit
    // PunchUnit is only used to be punched by OnePunch
    public Action selectAction(Map<String, Action> actionmap){
        return null;
    }
    public List<Integer> selectTargets(Action action, Troop[] troops){
        return null;
    }

}
