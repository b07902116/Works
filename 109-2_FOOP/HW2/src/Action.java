import java.util.List;

abstract public class Action {
    private int MPconsumption, target_num;
    private String name;
    public Action(int MPconsumption, int target_num, String name){
        this.MPconsumption = MPconsumption;
        this.target_num = target_num;
        this.name = name;
    }
    public int getMPconsumption(){
        return this.MPconsumption;
    }
    public int getTarget_num(){
        return this.target_num;
    }
    public String getName(){
        return this.name;
    }
    abstract public List<Unit> CandidateList(Unit caster, Troop[] troops);
    abstract public void Effect(Unit caster, Troop[] troops, List<Integer> targets);
}
