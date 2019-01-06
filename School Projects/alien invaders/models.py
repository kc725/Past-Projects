"""
Models module for Alien Invaders

This module contains the model classes for the Alien Invaders game. Anything that you
interact with on the screen is model: the ship, the laser bolts, and the aliens.

Just because something is a model does not mean there has to be a special class for
it.  Unless you need something special for your extra gameplay features, Ship and Aliens
could just be an instance of GImage that you move across the screen. You only need a new 
class when you add extra features to an object. So technically Bolt, which has a velocity, 
is really the only model that needs to have its own class.

With that said, we have included the subclasses for Ship and Aliens.  That is because
there are a lot of constants in consts.py for initializing the objects, and you might
want to add a custom initializer.  With that said, feel free to keep the pass underneath 
the class definitions if you do not want to do that.

You are free to add even more models to this module.  You may wish to do this when you 
add new features to your game, such as power-ups.  If you are unsure about whether to 
make a new class or not, please ask on Piazza.

Kevin Chen kc725
December 8 2017

Acknowledgements:
All png files used in this file were taken from the Images folder provided by
Walter White. 
"""
from consts import *
from game2d import *

# PRIMARY RULE: Models are not allowed to access anything in any module other than 
# consts.py.  If you need extra information from Gameplay, then it should be
# a parameter in your method, and Wave should pass it as a argument when it
# calls the method.


class Ship(GImage):
    """
    A class to represent the game ship.
    
    At the very least, you want a __init__ method to initialize the ships dimensions.
    These dimensions are all specified in consts.py.
    
    You should probably add a method for moving the ship.  While moving a ship just means
    changing the x attribute (which you can do directly), you want to prevent the player
    from moving the ship offscreen.  This is an ideal thing to do in a method.
    
    You also MIGHT want to add code to detect a collision with a bolt. We do not require
    this.  You could put this method in Wave if you wanted to.  But the advantage of 
    putting it here is that Ships and Aliens collide with different bolts.  Ships 
    collide with Alien bolts, not Ship bolts.  And Aliens collide with Ship bolts, not 
    Alien bolts. An easy way to keep this straight is for this class to have its own 
    collision method.
    
    However, there is no need for any more attributes other than those inherited by
    GImage. You would only add attributes if you needed them for extra gameplay
    features (like animation). If you add attributes, list them below.
    
    LIST MORE ATTRIBUTES (AND THEIR INVARIANTS) HERE IF NECESSARY
    """
    
    
    # GETTERS AND SETTERS (ONLY ADD IF YOU NEED THEM)
    def getX(self):
        return self.x
    def getY(self):
        return self.y
    
    # INITIALIZER TO CREATE A NEW SHIP
    def __init__(self,x,y,w,h,s):
        super().__init__(x=x,y=y,width=w,height=h,
                         source=s)
        
    # METHODS TO MOVE THE SHIP AND CHECK FOR COLLISIONS
    def movement(self,input):
        """
        Receives input from wave.update() to move the ship.
        """
        x = self.getX()
        y = self.getY()
        if input.is_key_down('left'):
            x -= SHIP_MOVEMENT
        if input.is_key_down('right'):
            x += SHIP_MOVEMENT
        if input.is_key_down('up'):
            y += SHIP_MOVEMENT
        if input.is_key_down('down'):
            y -= SHIP_MOVEMENT
            
        if x > GAME_WIDTH:
            x = GAME_WIDTH
        elif x < 0:
            x = 0
        if y+SHIP_HEIGHT/2 > DEFENSE_LINE:
            y = DEFENSE_LINE-SHIP_HEIGHT/2
        elif y-SHIP_HEIGHT/2< 0:
            y = SHIP_HEIGHT/2
        
        return Ship(x,y,SHIP_WIDTH,SHIP_HEIGHT,'ship.png')
    
    def collides(self,bolt):
        """
        Returns: True if the bolt was fired by an alien and collides with the ship
            
        Parameter bolt: The laser bolt to check
        Precondition: bolt is of class Bolt
        """
        if bolt.getEntity() > 0:
            return False
        else:
            x = bolt.getX()
            y = bolt.getY()
            topleft = (x-(BOLT_WIDTH/2),y+(BOLT_HEIGHT/2))
            topright = (x+(BOLT_WIDTH/2),y+(BOLT_HEIGHT/2))
            bottomleft = (x-(BOLT_WIDTH/2),y-(BOLT_HEIGHT/2))
            bottomright = (x+(BOLT_WIDTH/2),y-(BOLT_HEIGHT/2))
            
            status1 = self.contains(topleft) or self.contains(topright)
            status2 = self.contains(bottomleft) or self.contains(bottomright)
            
            return status1 or status2
        
        
    # ADD MORE METHODS (PROPERLY SPECIFIED) AS NECESSARY


class Alien(GImage):
    """
    A class to represent a single alien.
    
    At the very least, you want a __init__ method to initialize the alien dimensions.
    These dimensions are all specified in consts.py.
    
    You also MIGHT want to add code to detect a collision with a bolt. We do not require
    this.  You could put this method in Wave if you wanted to.  But the advantage of 
    putting it here is that Ships and Aliens collide with different bolts.  Ships 
    collide with Alien bolts, not Ship bolts.  And Aliens collide with Ship bolts, not 
    Alien bolts. An easy way to keep this straight is for this class to have its own 
    collision method.
    
    However, there is no need for any more attributes other than those inherited by
    GImage. You would only add attributes if you needed them for extra gameplay
    features (like giving each alien a score value). If you add attributes, list
    them below.
    
    LIST MORE ATTRIBUTES (AND THEIR INVARIANTS) HERE IF NECESSARY
    """
    
    
    # GETTERS AND SETTERS (ONLY ADD IF YOU NEED THEM)
    def getX(self):
        return self.x
    def getY(self):
        return self.y
    def getSource(self):
        return self.source
    # INITIALIZER TO CREATE AN ALIEN
    def __init__(self,x,y,w,h,s):
        super().__init__(x=x,y=y,width=w,height=h,source=s)
        
    # METHOD TO CHECK FOR COLLISION (IF DESIRED)
    def collides(self,bolt):
        """
        Returns: True if the bolt was fired by the player and collides with this alien
            
        Parameter bolt: The laser bolt to check
        Precondition: bolt is of class Bolt
        """
        if bolt.getEntity() < 0:
            return False
        else:
            x = bolt.getX()
            y = bolt.getY()
            topleft = (x-(BOLT_WIDTH/2),y+(BOLT_HEIGHT/2))
            topright = (x+(BOLT_WIDTH/2),y+(BOLT_HEIGHT/2))
            bottomleft = (x-(BOLT_WIDTH/2),y-(BOLT_HEIGHT/2))
            bottomright = (x+(BOLT_WIDTH/2),y-(BOLT_HEIGHT/2))
            
            status1 = self.contains(topleft) or self.contains(topright)
            status2 = self.contains(bottomleft) or self.contains(bottomright)
            
            return status1 or status2
        
    # ADD MORE METHODS (PROPERLY SPECIFIED) AS NECESSARY
    def hshift(self,direction):
        """
        Shifts an alien horizontally in either left or right direction by ALIEN_H_WALK.
        
        Parameters:
        direction: the direction to move in (1 for right, -1 for left)
        """
        x = self.getX() + (direction*ALIEN_H_WALK)
        return Alien(x,self.y,ALIEN_WIDTH,ALIEN_HEIGHT,self.source)
    
    def vshift(self,direction):
        """
        Shifts an alien vertically down while also shifting them back horizontally
        to keep them completely on screen.
        
        Parameters:
        direction: the direction to move back in (1 for right, -1 for left)
        """
        x = self.getX() - (direction*ALIEN_H_WALK)
        y = self.getY() - ALIEN_V_WALK
        return Alien(x,y,ALIEN_WIDTH,ALIEN_HEIGHT,self.source)
    
    
class Bolt(GRectangle):
    """
    A class representing a laser bolt.
    
    Laser bolts are often just thin, white rectangles.  The size of the bolt is 
    determined by constants in consts.py. We MUST subclass GRectangle, because we
    need to add an extra attribute for the velocity of the bolt.
    
    The class Wave will need to look at these attributes, so you will need getters for 
    them.  However, it is possible to write this assignment with no setters for the 
    velocities.  That is because the velocity is fixed and cannot change once the bolt
    is fired.
    
    In addition to the getters, you need to write the __init__ method to set the starting
    velocity. This __init__ method will need to call the __init__ from GRectangle as a 
    helper.
    
    You also MIGHT want to create a method to move the bolt.  You move the bolt by adding
    the velocity to the y-position.  However, the getter allows Wave to do this on its
    own, so this method is not required.
    
    INSTANCE ATTRIBUTES:
        _velocity: The velocity in y direction [int or float]
    
    LIST MORE ATTRIBUTES (AND THEIR INVARIANTS) HERE IF NECESSARY
        _entity: The model that shot the bolt, also determines the direction of
                 the velocity [int greater than or less than 0].
    """
    
    # GETTERS AND SETTERS (ONLY ADD IF YOU NEED THEM)
    def getX(self):
        return self.x
    def getY(self):
        return self.y
    def getV(self):
        return self._velocity
    def getEntity(self):
        return self._entity
    # INITIALIZER TO SET THE VELOCITY
    def __init__(self,x,y,entity):
        if entity>0:
            color = 'blue'
        else:
            color = 'red'
        super().__init__(x=x,y=y,width=BOLT_WIDTH,height=BOLT_HEIGHT,
                         fillcolor=color,) 
        self._velocity = entity*BOLT_SPEED
        self._entity = entity
        
        
    # ADD MORE METHODS (PROPERLY SPECIFIED) AS NECESSARY

# IF YOU NEED ADDITIONAL MODEL CLASSES, THEY GO HERE