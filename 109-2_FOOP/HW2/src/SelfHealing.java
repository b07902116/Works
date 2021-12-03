import java.util.List;

public class SelfHealing extends Action{
    public SelfHealing(){
        super(50, 0, "SelfHealing");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        return null;
    }
    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        int heal = 150;
        caster.setMP(caster.getMP() - getMPconsumption());
        caster.setHP(caster.getHP() + heal);
        System.out.printf("%s uses %s.\n", caster.getName(), this.getName());
    }
}
