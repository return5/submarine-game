# version 1.0
## intro
- a turned based submarine combat game of exteme action and excitment. an absolute adrenline rush

## objective
 - destroy the enmy submarines, destroyers, and cargo ship. dont get destroyed yourself
 
 ## game pieces
  - ### player
    - player controls a submarine
    - can move in all directions, incluing up and down
    - armed with regular torpedoes and 2 area fo effect torpedoes with 5 block blast radius
    - has latest in sonar technology
    - initial placement is randomly in upper left hand quadrant
   
  - ### enemy submarine
    - has the same capabilities as player submarine
    - initial placement is randomly in bottom left quadrant
   
  - ### enemy destroyer
    - can move only on the surface of the water
    - has depth charges with blast radius of 5 capable of reaching any depth
    - also has a cannon for firing at surface targets
    - has sonar system of its own
    - inital placement is a random location any where on the map
  - ### cargo ship
    - has no weapons nor sonar of its own
     
  ## difficulty selection
   - upon starting the game the player is given a few options for difficulty
   - ### easy
      - one enemy destroyer and one cargo ship
   - ### normal
      - same as above and the addition of one enemy sub
   - ### hard
     - same as previous two with the addition of a second enemy destroyer
   - ### very hard
      - same as hard but with the addition of a second enemy submarine.
     
  ## screenshots
  ![screenshot1](/screenshots/diff_select.png)
  ![screenshot2](/screenshots/aoetor.png)
  ![screenshot3](/screenshots/sonar.png)
  ![screenshot4](/screenshots/destroyenemysub.png)
   
## to do
  - [x] add ability to move player sub
  - [x] add ability to fire torpedos and sink enemy ships
  - [x] add sonar to detect enemy ships
  - [x] add ability to be damaged by depth charges and AOE torpedoes
  - [x] add turns for each player
  - [x] add decision tree for enemy computer 
  - [ ] fix up game display 
  - [ ] clean up code
  - [ ] sinking enemy cargo ship sets all enemy ships after you
