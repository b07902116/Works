import java.util.ArrayList;
import java.util.List;

public class Curse extends Action{
    public Curse(){
        super(100, 1, "Curse");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        Troop enemy_troop = troops[caster.getTroop_index() % 2];
        return enemy_troop.LivingUnitList();
    }
    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        Troop enemy_troop = troops[caster.getTroop_index() % 2];
        Unit target;
        if (targets_index != null)
            target = enemy_troop.LivingUnitList().get(targets_index.get(0));
        else
            target = enemy_troop.LivingUnitList().get(0);

        caster.setMP(caster.getMP() - this.getMPconsumption());
        target.getCurse_list().add(caster.getUnit_index());
        System.out.printf("%s uses %s on %s.\n", caster.getName(), this.getName(), target.getName());
    }
}
