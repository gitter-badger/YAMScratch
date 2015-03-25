
class GuillotineNode(object):
    def __init__(self, x0, y0, width, height):
        self.width = width
        self.height = height
        #define in global coordinates where this node is
        self.x0 = x0
        self.y0 = y0
        self.used = False
        #tree pointers
        self.up = None
        self.right = None

    def __str__(self):
        result = "x0: " + str(self.x0) + " y0: " + str(self.y0) + '\n'
        result += (str(self.width) + " " + str(self.height) + '\n')
        return result

    @classmethod
    def findNode(cls ,root, w, h):
        if(root.used):
            found_node = GuillotineNode.findNode(root.right, w, h)
            if found_node:
                return found_node
            #try the other node
            other_node = GuillotineNode.findNode(root.up, w, h)
            if other_node:
                return other_node
            #if neither node has room, then return None
            #we coerce the truth value of None a lot in this function
            else:
                return None

        elif (w <= root.width and h <= root.height):
            return root
        else:
            #return null if the node is not used but the box does not fit
            return None

    def splitNode(self, w_box, h_box):
        '''
        depiction below of splitting procedure
        +-----------------------+
        |                       |
        |         up            |
        |                       |
        +-------+---------------+   
        |       |               |
        | root  |     right     |
        +-------+---------------+
        the root node is the current node, which creates two child nodes
        when a box is inserted into the root. The root node becomes the
        same size as the box going in
        '''
        #create the two child nodes
        self.up = GuillotineNode(self.x0, (self.y0 + h_box), self.width, (self.height - h_box))
        self.right = GuillotineNode(self.x0 + w_box, self.y0, (self.width - w_box), h_box)
        self.used = True
        #we return self so the layer object can store a reference to it
        return self