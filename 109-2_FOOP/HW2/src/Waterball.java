import java.util.List;

public class Waterball extends Action{
    public Waterball(){
        super(50, 1, "Waterball");
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

        int damage = 120 + caster.getExtra_dmg();
        caster.setMP(caster.getMP() - getMPconsumption());
        target.setHP(target.getHP() - damage);
        System.out.printf("%s uses %s on %s.\n", caster.getName(), this.getName(), target.getName());
        System.out.printf("%s causes %d damage to %s.\n", caster.getName(), damage, target.getName());
        if (target.getHP() <= 0){
            target.DeathTask(troops);
        }
    }
}
