import java.util.List;

public class Petrochemical extends Action{
    public Petrochemical(){
        super(100, 1, "Petrochemical");
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
        target.setState("Petrochemical");
        target.setCan_move(false);
        target.setDot(0);
        target.setExtra_dmg(0);
        target.setDuration(3);
        System.out.printf("%s uses %s on %s.\n", caster.getName(), this.getName(), target.getName());
    }
}
