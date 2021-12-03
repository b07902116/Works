import java.util.ArrayList;
import java.util.List;

public class Cheerup extends Action{
    public Cheerup(){
        super(100, 3, "Cheerup");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        Troop ally_troop = troops[caster.getTroop_index() - 1];
        List<Unit> candidate = ally_troop.LivingUnitList();
        candidate.remove(caster);
        return candidate;

    }
    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        Troop ally_troop = troops[caster.getTroop_index() - 1];
        List<Unit> targets = new ArrayList<>();
        if (targets_index != null){
            List<Unit> living_units = ally_troop.LivingUnitList();
            living_units.remove(caster);
            for (int i = 0; i < targets_index.size(); i++){
                targets.add(living_units.get(targets_index.get(i)));
            }
        }
        else {
            targets = ally_troop.LivingUnitList();
            targets.remove(caster);
        }

        caster.setMP(caster.getMP() - this.getMPconsumption());
        System.out.printf("%s uses %s on", caster.getName(), this.getName());
        for (int i = 0; i < targets.size(); i++){
            if (i == (targets.size() - 1))
                System.out.printf(" %s.\n", targets.get(i).getName());
            else
                System.out.printf(" %s,", targets.get(i).getName());
        }
        for (int i = 0; i < targets.size(); i++){
            Unit target = targets.get(i);
            target.setState("Cheerup");
            target.setCan_move(true);
            target.setDot(0);
            target.setExtra_dmg(50);
            target.setDuration(3);
        }

    }
}
