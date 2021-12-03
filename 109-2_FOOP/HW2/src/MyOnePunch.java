import tw.waterball.foop.hw2.provided.OnePunch;

import java.util.List;

public class MyOnePunch extends Action{
    final OnePunch punch;
    public MyOnePunch(){
        super(180, 1, "OnePunch");
        punch = new OnePunch();
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

        String[] tokens = new String[4];
        tokens[0] = String.valueOf(target.getName());
        tokens[1] = String.valueOf(target.getHP());
        tokens[2] = String.valueOf(target.getMP());
        tokens[3] = String.valueOf(target.getSTR());
        PunchUnit punch_unit = new PunchUnit(target.getTroop_index(), target.getUnit_index(), tokens);
        punch.perform(punch_unit);
        int damage = target.getHP() - punch_unit.getHP() + caster.getExtra_dmg();
        caster.setMP(caster.getMP() - getMPconsumption());
        target.setHP(target.getHP() - damage);
        System.out.printf("%s uses %s on %s.\n", caster.getName(), this.getName(), target.getName());
        System.out.printf("%s causes %d damage to %s.\n", caster.getName(), damage, target.getName());
        if (target.getHP() <= 0){
            target.DeathTask(troops);
        }
    }
}
