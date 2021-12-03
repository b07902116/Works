# Design Report
![FOOP](FOOP.png)

## Software Design

- Follow the Open-Close Principle on removing/adding the skill SelfHealing  
  SelfHealing class extends Action class.  
  Adding SelfHealing only needs to implement the class and add it in Main class

  
- Follow the Open-Close Principle on removing/adding the skill Summon  
  Summon class extends Action class.  
  Adding Summon only needs to implement the class and add it in Main class
  

- Follow the Open-Close Principle on removing/adding the skill Petrochemical  
  Petrochemical class extends Action class.  
  Adding Petrochemical only needs to implement the class and add it in Main class


## Bonus Design

- Follow the Open-Close Principle on removing/adding the skill Curse  
  Curse class extends Action class.  
  Adding Curse only needs to implement the class and add it in Main class


- Follow the Open-Close Principle on removing/adding the skill Cheerup  
  Cheerup class extends Action class.  
  Adding Cheerup only needs to implement the class and add it in Main class
  

- Follow the Open-Close Principle on removing/adding the skill OnePunch  
  MyOnePunch class extends Action class.  
  PunchUnit class extends Unit class and implements Target class.  
  MyOnePunch new a PunchUnit and let it use OnePunch.
  Then the damage caused to PunchUnit can be calculated and punch the read unit.  
  Adding OnePunch only needs to implement MyOnePunch, PunchUnit classes and add MyOnePunch in Main class
  
  

