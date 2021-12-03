import java.util.ArrayList;
import java.util.List;

public class SelfExplosion extends Action{
    public SelfExplosion(){
        super(200, 0, "SelfExplosion");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        return null;
    }
    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        List<Unit> targets = new ArrayList<>();
        targets.addAll(troops[0].LivingUnitList());
        targets.addAll(troops[1].LivingUnitList());
        targets.remove(caster);

        int damage = 150 + caster.getExtra_dmg();
        caster.setMP(caster.getMP() - getMPconsumption());
        System.out.printf("%s uses %s on", caster.getName(), this.getName());
        for (int i = 0; i < targets.size(); i++){
            Unit target = targets.get(i);
            if (i == (targets.size() - 1))
                System.out.printf(" %s.\n", target.getName());
            else
                System.out.printf(" %s,", target.getName());
        }
        for (int i = 0; i < targets.size(); i++){
            Unit target = targets.get(i);
            target.setHP(target.getHP() - damage);
            System.out.printf("%s causes %d damage to %s.\n", caster.getName(), damage, target.getName());
            if (target.getHP() <= 0){
                target.DeathTask(troops);
            }
        }
        caster.setHP(0);
        caster.DeathTask(troops);
    }
}
