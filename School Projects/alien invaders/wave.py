"""
Subcontroller module for Alien Invaders

This module contains the subcontroller to manage a single level or wave in the Alien
Invaders game.  Instances of Wave represent a single wave.  Whenever you move to a
new level, you are expected to make a new instance of the class.

The subcontroller Wave manages the ship, the aliens and any laser bolts on screen.  
These are model objects.  Their classes are defined in models.py.

Most of your work on this assignment will be in either this module or models.py.
Whether a helper method belongs in this module or models.py is often a complicated
issue.  If you do not know, ask on Piazza and we will answer.

Kevin Chen kc725
December 8 2017

Acknowledgements:
All png and sound files used in this file were taken from the Images and Sound folders
provided by Walter White. 
"""
from game2d import *
from consts import *
from models import *
import random

# PRIMARY RULE: Wave can only access attributes in models.py via getters/setters
# Wave is NOT allowed to access anything in app.py (Subcontrollers are not permitted 
# to access anything in their parent. To see why, take CS 3152)


class Wave(object):
    """
    This class controls a single level or wave of Alien Invaders.
    
    This subcontroller has a reference to the ship, aliens, and any laser bolts on screen. 
    It animates the laser bolts, removing any aliens as necessary. It also marches the
    aliens back and forth across the screen until they are all destroyed or they reach
    the defense line (at which point the player loses). When the wave is complete, you 
    should create a NEW instance of Wave (in Invaders) if you want to make a new wave of 
    aliens.
    
    If you want to pause the game, tell this controller to draw, but do not update.  See 
    subcontrollers.py from Lecture 24 for an example.  This class will be similar to
    than one in how it interacts with the main class Invaders.
    
    #UPDATE ME LATER
    INSTANCE ATTRIBUTES:
        _ship:   the player ship to control [Ship]
        _aliens: the 2d list of aliens in the wave [rectangular 2d list of Alien or None] 
        _bolts:  the laser bolts currently on screen [list of Bolt, possibly empty]
        _dline:  the defensive line being protected [GPath]
        _lives:  the number of lives left  [int >= 0]
        _time:   The amount of time since the last Alien "step" [number >= 0]
    
    As you can see, all of these attributes are hidden.  You may find that you want to
    access an attribute in class Invaders. It is okay if you do, but you MAY NOT ACCESS 
    THE ATTRIBUTES DIRECTLY. You must use a getter and/or setter for any attribute that 
    you need to access in Invaders.  Only add the getters and setters that you need for 
    Invaders. You can keep everything else hidden.
    
    You may change any of the attributes above as you see fit. For example, may want to 
    keep track of the score.  You also might want some label objects to display the score
    and number of lives. If you make changes, please list the changes with the invariants.
    
    LIST MORE ATTRIBUTES (AND THEIR INVARIANTS) HERE IF NECESSARY
    _direction: the direction the alien wave moves in [either 1 or -1]
    _lastkey: whether the z key was pressed in the previous animation frame [bool]
    _playerbolt: whether there is a player bolt currently on the screen [bool]
    _aliensteps: the number of steps the aliens have moved since last shooting [int >= 0]
    _alienshoot: the number of steps before a random alien shoots [int between 1 and BOLT_RATE]
    message: communicates to Invader to change states [one of the _state constants]
    win: communicates to Invader whether the player has won or lost [bool]
    sounds: whether there should be sounds playing or not [bool]
    _ALIEN_SPEED: The current speed of the aliens [int or float]
    """
    
    # GETTERS AND SETTERS (ONLY ADD IF YOU NEED THEM)
    
    
    # INITIALIZER (standard form) TO CREATE SHIP AND ALIENS
    def __init__(self):
        self._aliens = []
        if ALIEN_ROWS%2 == 0:
            a = 0
        else:
            a = 1
        for y in range(ALIEN_ROWS):
            onewave = []
            for x in range(ALIENS_IN_ROW):
                h = (ALIEN_H_SEP*(x+1)) + (ALIEN_WIDTH/2) + ALIEN_WIDTH*x
                v = GAME_HEIGHT - ALIEN_CEILING - ALIEN_V_SEP*y - ALIEN_HEIGHT*y
                source = ALIEN_IMAGES[a//2]
                onewave.append(Alien(h,v,ALIEN_WIDTH,ALIEN_HEIGHT,source))
            self._aliens.append(onewave)
            a += 1
            if a>5:
                a = 0
                
        self._ship = Ship(GAME_WIDTH/2,SHIP_BOTTOM,SHIP_WIDTH,SHIP_HEIGHT,
                          'ship.png')
        
        self._dline = GPath(points=[0,DEFENSE_LINE,GAME_WIDTH,DEFENSE_LINE],
                            linewidth=1,linecolor='gray')
        
        self._time = 0
        self._direction = 1
        self._bolts = []
        self._lastkey = True
        self._playerbolt = False
        self._aliensteps = 0
        self._alienshoot = random.randint(1,BOLT_RATE)
        self._lives = 3
        self.message = STATE_ACTIVE
        self.sounds = True
        self._ALIEN_SPEED = ALIEN_SPEED
        
    # UPDATE METHOD TO MOVE THE SHIP, ALIENS, AND LASER BOLTS
    def update(self,input,dt):
        
        #Turning the sounds on or off
        if input.is_key_down('m') and self._lastkey == False:
            if self.sounds:
                self.sounds = False
            else:
                self.sounds = True
        
        #MOVING THE SHIP
        if not self._ship is None:
            self._ship = self._ship.movement(input)
        
        #MOVING THE ALIENS
        self._time += dt
        
        if self._time >= self._ALIEN_SPEED:
            self.moveAliens()
            self._time = 0
            self._aliensteps += 1
        
        #PLAYER SHOOTING BOLT
        if not self._ship is None:
            if (input.is_key_down('z') and self._lastkey == False and
                self._playerbolt == False):
                self.PlayerShoot()
                if self.sounds:
                    sound = Sound('pew1.wav')
                    sound.volume = 0.3
                    sound.play()
                    
            self._lastkey = input.is_key_down('z')
        
        #ALIEN SHOOTING BOLT
        if self._aliensteps >= self._alienshoot:
            self.AlienShoot()

        #FIRING ALL THE BOLTS
        self.FireBolts()
        
        #CHECK BOLT COLLISIONS
        self.collisions()
                        
        #CHECK PLAYER LIVES
        self.isplayeralive()
        
        #CHECK IF THE ALIENS REACHED THE LINE
        self.finishline()
                
        #CHECK IF THE LIST OF ALIENS IS EMPTY
        self.aliensleft()
        
    # DRAW METHOD TO DRAW THE SHIP, ALIENS, DEFENSIVE LINE AND BOLTS
    def draw(self,view):
        for x in range(len(self._aliens)):
            for y in range(len(self._aliens[x])):
                if not self._aliens[x][y] is None:
                    self._aliens[x][y].draw(view)
        
        if not self._ship is None:
            self._ship.draw(view)
        self._dline.draw(view)
        for x in range(len(self._bolts)):
            self._bolts[x].draw(view)
    
    # HELPER METHODS FOR COLLISION DETECTION
    def collisions(self):
        "Checks for any collisions"
        
        todelete1 = []
        
        for x in range(len(self._bolts)):
            
            if not self._ship is None:
                
                if self._ship.collides(self._bolts[x]):
                    todelete1.append(x)
                    self._ship = None
                    self._lives -= 1
                    if self.sounds:
                        sound = Sound('blast1.wav')
                        sound.volume=0.3
                        sound.play()
                
            for a in range(len(self._aliens)):
                for b in range(len(self._aliens[a])):
                    if not self._aliens[a][b] is None:
                        if self._aliens[a][b].collides(self._bolts[x]):
                            todelete1.append(x)
                            self._aliens[a][b] = None
                            self._playerbolt = False
                            if self.sounds:
                                num = random.randint(2,3)
                                sound = Sound('blast'+str(num)+'.wav')
                                sound.volume = 0.3
                                sound.play()
                            self._ALIEN_SPEED = 0.98*self._ALIEN_SPEED
        for x in todelete1:
            del self._bolts[x]
    
    #OTHER HELPER METHODS
    def moveAliens(self):
        """
        Moves self._aliens in the right directions every time dt reaches ALIEN_SPEED.
        """
        
        #MOVE EACH ALIEN HORIZONTALLY IN THE RIGHT DIRECTION
        for x in range(len(self._aliens)):
            for y in range(len(self._aliens[x])):
                if not self._aliens[x][y] is None:
                    self._aliens[x][y] = self._aliens[x][y].hshift(self._direction)
                    
        #DETERMINES THE RIGHTMOST/LEFTMOST ALIEN
        if self._direction > 0:
            check = 0
            for x in range(len(self._aliens)):
                for y in range(len(self._aliens[x])):
                    if not self._aliens[x][y] is None:
                        check = max(check,self._aliens[x][y].getX())
        else:
            check = GAME_WIDTH*2
            for x in range(len(self._aliens)):
                for y in range(len(self._aliens[x])):
                    if not self._aliens[x][y] is None:
                        check = min(check,self._aliens[x][y].getX())
            
        #CHECKS TO SEE IF THE ALIENS NEED TO SHIFT DOWN
        if (check+(ALIEN_WIDTH/2)>=GAME_WIDTH) or (check-(ALIEN_WIDTH/2)<=0):
            for x in range(len(self._aliens)):
                for y in range(len(self._aliens[x])):
                    if not self._aliens[x][y] is None:
                        self._aliens[x][y]=self._aliens[x][y].vshift(
                            self._direction)
            
            self._direction = self._direction * (-1)
                
    def PlayerShoot(self):
        "Adds a player Bolt to self._bolts"
        x = self._ship.getX()
        y = self._ship.getY()+(SHIP_HEIGHT/2)
        bolt = Bolt(x,y,1)
        self._bolts.append(bolt)
        self._playerbolt = True

    def AlienShoot(self):
        "Adds an Alien Bolt to self._bolts"
        bolt = None
        columns = []
        for x in range(ALIENS_IN_ROW):
            counter = 0
            for y in range(ALIEN_ROWS):
                if not self._aliens[y][x] is None:
                    counter += 1
            if counter > 0:
                columns.append(x)
        if len(columns) == 0:
            pass
        else:
            randcol = random.choice(columns)
            for x in reversed(range(ALIEN_ROWS)):
                if self._aliens[x][randcol] is None or not bolt is None:
                    pass
                else:
                    posx = self._aliens[x][randcol].getX()
                    posy = self._aliens[x][randcol].getY() - (ALIEN_HEIGHT/2)
                    bolt = Bolt(posx,posy,(-1))
                    self._bolts.append(bolt)
                    if self.sounds:
                        sound = Sound('pew2.wav')
                        sound.volume = 0.3
                        sound.play()
                        
            self._aliensteps = 0
            self._alienshoot = random.randint(1,BOLT_RATE)
        
    def FireBolts(self):
        "Moves every bolt in self._bolts."
        todelete2 = []
        playerbolt = False
        for a in range(len(self._bolts)):
            x = self._bolts[a].getX()
            entity = self._bolts[a].getEntity()
            velocity = self._bolts[a].getV()
            y = self._bolts[a].getY() + velocity
            if y-(BOLT_HEIGHT/2)>=GAME_HEIGHT:
                todelete2.append(a)
                playerbolt = True
            elif y+(BOLT_HEIGHT/2)<=0:
                todelete2.append(a)
            else:
                self._bolts[a] = Bolt(x,y,entity)
            
        for x in todelete2:
            try:
                del self._bolts[x]
            except:
                pass
            
        if playerbolt:
            self._playerbolt = False
            
    def resume(self):
        "Continues the game where it left off"
        self._lastkey = True
        self._ship = Ship(GAME_WIDTH/2,SHIP_BOTTOM,SHIP_WIDTH,SHIP_HEIGHT,
                          'ship.png')
        self.message = STATE_ACTIVE
        
    def isplayeralive(self):
        "Checks to see if the player ship is None and if they lost all their lives."
        if self._ship is None:
            if self._lives > 0:
                self.message = STATE_PAUSED
            else:
                self.win = False
                self.message = STATE_COMPLETE
    
    def finishline(self):
        "Checks to see if the aliens have breached the defense line."
        bottom = GAME_HEIGHT
        for x in range(len(self._aliens)):
            for y in range(len(self._aliens[x])):
                if not self._aliens[x][y] is None:
                    bottom = self._aliens[x][y].getY() - (ALIEN_HEIGHT/2)
        
        if bottom <= DEFENSE_LINE:
            self.win = False
            self.message = STATE_COMPLETE
            
    def aliensleft(self):
        "Checks to see if there are any aliens left alive (not None)."
        count = 0
        for x in range(len(self._aliens)):
            for y in range(len(self._aliens[x])):
                if not self._aliens[x][y] is None:
                    count += 1
        
        if count == 0:
            self.win = True
            self.message = STATE_COMPLETE