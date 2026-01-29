from direct.showbase.ShowBase import ShowBase
from panda3d.core import Geom, GeomNode, GeomVertexFormat, GeomVertexData, GeomVertexWriter, GeomTriangles
from panda3d.core import TextureStage, AmbientLight, DirectionalLight, LVector3
import math

class PlanetDemo(ShowBase):
    def __init__(self):
        ShowBase.__init__(self)

        # Generate a sphere procedurally
        self.planet = self.create_sphere(1.0, 32, 32)
        self.planet.reparentTo(self.render)

        # Load and apply the texture (ensure you have a texture file)
        self.planet_tex = self.loader.loadTexture("earth_texture.jpg")
        self.planet.setTexture(self.planet_tex)

        # Set the scale and initial position
        self.planet.setScale(2, 2, 2)
        self.planet.setPos(0, 10, 0)

        # Lighting
        dlight = DirectionalLight('dlight')
        dlight.setColor((0.8, 0.8, 0.8, 1))
        dlightNP = self.render.attachNewNode(dlight)
        dlightNP.setHpr(0, -60, 0)
        self.render.setLight(dlightNP)

        alight = AmbientLight('alight')
        alight.setColor((0.2, 0.2, 0.2, 1))
        alightNP = self.render.attachNewNode(alight)
        self.render.setLight(alightNP)

        # Rotate the planet continuously
        self.taskMgr.add(self.rotatePlanetTask, "RotatePlanetTask")

    def rotatePlanetTask(self, task):
        angleDegrees = task.time * 10.0
        self.planet.setHpr(angleDegrees, 0, 0)
        return task.cont

    def create_sphere(self, radius, slices, stacks):
        format = GeomVertexFormat.getV3n3t2()
        vdata = GeomVertexData('sphere', format, Geom.UHStatic)

        vertex = GeomVertexWriter(vdata, 'vertex')
        normal = GeomVertexWriter(vdata, 'normal')
        texcoord = GeomVertexWriter(vdata, 'texcoord')

        for i in range(stacks + 1):
            lat = math.pi * (-0.5 + float(i) / stacks)
            for j in range(slices + 1):
                lon = 2 * math.pi * float(j) / slices

                x = radius * math.cos(lat) * math.cos(lon)
                y = radius * math.cos(lat) * math.sin(lon)
                z = radius * math.sin(lat)

                u = float(j) / slices
                v = float(i) / stacks

                vertex.addData3(x, y, z)
                normal.addData3(x, y, z)
                texcoord.addData2(u, v)

        triangles = GeomTriangles(Geom.UHStatic)

        for i in range(stacks):
            for j in range(slices):
                p1 = i * (slices + 1) + j
                p2 = p1 + slices + 1
                p3 = p1 + 1
                p4 = p2 + 1

                triangles.addVertices(p1, p3, p2)
                triangles.addVertices(p3, p4, p2)

        geom = Geom(vdata)
        geom.addPrimitive(triangles)
        node = GeomNode('gnode')
        node.addGeom(geom)

        return self.render.attachNewNode(node)

# Start the application
app = PlanetDemo()
app.run()
