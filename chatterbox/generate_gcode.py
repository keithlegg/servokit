
"""
    protocol to get more complex data out of linuxCNC 

    set the 4 digital IO pins. The last pin triggers a pin change interrupt to load all in parallel 


    TO send data: 

    use 4 gcodes per frame 
        4 bits in "parallel"

        set 0
        set 1
        set 2
        set 3 <- transistion triggers the transfer 

"""


class protocol(object):
    """ pin map is a dict that has ON and OFF command for an indexed pin, typically 4 (bits)
    """
    
    def __init__(self):
        self.com  = {} #name byte(command)   
        self.pmap = {} #int, gcodeON gcodeOFF
    
    def map_pin(self, pm_key, pm_val):
        self.pmap[pm_key] = pm_val

    def add_command(self, name, bytecom ):
        self.com[name] = bytecom

    
    def show_commands(self):
        for key, value in self.com.items():
            print(key+" "+ '0x%s'%bytearray([value]).hex())
 
     
    def exists(self, name):
        if name in self.com:
            return self.com[name] 
        else:
            return False    
     





####################################
class gen_gcode(object):
    """
        test of system to embed encoded bytes in gcode 
    """

    def __init__(self, pinmap):
        self.pm = pinmap
        self.commands = []
        self.gcode    = []
    

    def add(self, name, value):
        com = self.pm.exists(name)
        if com:
            self.commands.append([ name, com, value ])
            print('added new command: %s %s'%(name,value))
        else:
            print('command %s does not exist in protocol'%name)


    def split_4bit(self, int_com):
        for i in range(4):
            if(int_com>>i & 0x01):
                print('YES!')
            else:
                print('NO')    



    def export(self, filename):
        for c in self.commands:
            comstr=  ''
            self.gcode.append('#com %s'%c[0] )
            
            com_int = self.pm.com[c[0]]
            
            self.split_4bit(com_int)



            #comstr+= '%s'%self.pm.com[c[0]] #pin on 
            #comstr+= '%s'%self.pm.com[c[1]] #pin off 

            #self.gcode.append(self.pm.pmap[c[1]]      )           



        f = open(filename, "w")
        for l in self.gcode:
            f.write('%s\n'%l)
        f.close()

        



####################################

  
pc = protocol()

pc.map_pin(0,['M64 P0', 'M65 P0'])
pc.map_pin(1,['M64 P1', 'M65 P1'])
pc.map_pin(2,['M64 P2', 'M65 P2'])
pc.map_pin(3,['M64 P3', 'M65 P3'])

pc.add_command('linethick', 0xa )
pc.add_command('headup'   , 0x1 )
pc.add_command('headdown' , 0x2 )

#print(pc.pinmap)
pc.show_commands()

 
gc = gen_gcode(pinmap=pc)
gc.add('headdown', 100 )


gc.export('my_file.ngc')
 





