import java.util.List;

public class Fireball extends Action{
    public Fireball(){
        super(50, 0, "Fireball");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        return null;
    }

    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        Troop enemy_troop = troops[caster.getTroop_index() % 2];
        List<Unit> targets = enemy_troop.LivingUnitList();
        int damage = 50 + caster.getExtra_dmg();
        caster.setMP(caster.getMP() - getMPconsumption());
        System.out.printf("%s uses %s on", caster.getName(), this.getName());
        for (int i = 0; i < targets.size(); i++){
            if (i == (targets.size() - 1))
                System.out.printf(" %s.\n", targets.get(i).getName());
            else
                System.out.printf(" %s,", targets.get(i).getName());
        }
        for (int i = 0; i < targets.size(); i++){
            Unit target = targets.get(i);
            target.setHP(target.getHP() - damage);
            System.out.printf("%s causes %d damage to %s.\n", caster.getName(), damage, target.getName());
            if (target.getHP() <= 0){
                target.DeathTask(troops);
            }
        }
    }
}
