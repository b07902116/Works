import java.util.List;

public class Poison extends Action{
    public Poison(){
        super(80, 1, "Poison");
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
        target.setState("Poisoned");
        target.setCan_move(true);
        target.setDot(30);
        target.setExtra_dmg(0);
        target.setDuration(3);
        System.out.printf("%s uses %s on %s.\n", caster.getName(), this.getName(), target.getName());
    }
}
