import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class RPG {
    Troop[] troops;
    public RPG(){
        troops = new Troop[2];
        troops[0] = new Troop(1);
        troops[1] = new Troop(2);
    }
    public void GameStart(Map<String, Action> actionmap){
        Input.InputTroop(troops[0]);
        Input.InputTroop(troops[1]);
        while (true){
            for (int i = 0; i < 2; i++){
                List<Unit> living_units = troops[i].LivingUnitList();
                for (int j = 0; j < troops[i].getLiving_unit_num(); j++){
                    Unit unit = living_units.get(j);
                    unit.OneRoundPass();
                }
            }
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < troops[i].getLiving_unit_num(); j++){
                    List<Unit> living_units = troops[i].LivingUnitList();
                    Unit acting_unit = living_units.get(j);
                    acting_unit.ShowUnitInfo();
                    if (!acting_unit.getCan_move()){
                        continue;
                    }
                    // take dot damage
                    acting_unit.setHP(acting_unit.getHP() - acting_unit.getDot());
                    if (acting_unit.getHP() <= 0){
                        acting_unit.DeathTask(troops);
                        if (GameOver())
                            return;
                        continue;
                    }
                    Action action = acting_unit.selectAction(actionmap);

                    List<Integer> targets = acting_unit.selectTargets(action, troops);

                    action.Effect(acting_unit, troops, targets);
                    if (acting_unit.getHP() <= 0){
                        j--;
                    }
                    if (GameOver())
                        return;
                }
            }


        }
    }
    private boolean GameOver(){
        troops[0].UpdateLivingUnitNum();
        troops[1].UpdateLivingUnitNum();
        if (troops[0].getUnit(0).getHP() <= 0){
            System.out.println("You lose.");
            return true;
        }
        else if (troops[1].getLiving_unit_num() == 0){
            System.out.println("You win.");
            return true;
        }
        else{
            return false;
        }
    }
}
