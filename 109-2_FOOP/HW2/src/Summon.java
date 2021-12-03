import java.util.List;

public class Summon extends Action{
    public Summon(){
        super(150, 0, "Summon");
    }
    public List<Unit> CandidateList(Unit caster, Troop[] troops){
        return null;
    }
    public void Effect(Unit caster, Troop[] troops, List<Integer> targets_index){
        Troop ally_troop = troops[caster.getTroop_index() - 1];
        caster.setMP(caster.getMP() - this.getMPconsumption());
        String[] tokens = new String[4];
        tokens[0] = "Slime";
        tokens[1] = "100";
        tokens[2] = "0";
        tokens[3] = "50";
        ally_troop.AddUnit(tokens);
        System.out.printf("%s uses %s.\n", caster.getName(), this.getName());
    }
}
