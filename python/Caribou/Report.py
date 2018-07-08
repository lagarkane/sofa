import os
import math
import tempfile
import base64

from .View import ParaView
from .Utils import bbox
from .Mesh import Mesh


class HtmlReport(object):
    def __init__(self, **kwargs):
        # Parameters
        self.name = kwargs.get('name')
        assert not self.name == ""

        # Members
        self.lines = []
        self.lines.append('<html>')
        self.lines.append('<head>')
        self.lines.append('<title>{}</title>'.format(self.name))

        self.lines.append("""
        <style>
            table {
                font-family: arial, sans-serif;
                border-collapse: collapse;
                width: 100%;
            }
            
            td, th {
                border: 1px solid #dddddd;
                text-align: left;
                padding: 8px;
            }
            
            tr:nth-child(even) {
                background-color: #dddddd;
            }
            
            /* Style the buttons that are used to open and close the accordion panel */
            .accordion {
                background-color: #eee;
                color: #444;
                cursor: pointer;
                padding: 18px;
                width: 100%;
                text-align: left;
                border: none;
                outline: none;
                transition: 0.4s;
            }
            
            .accordion.section {
            background-color: #000;
            color: #FFF;
            font-weight: bold;
            }
            

            button.accordion::after {
            
                content: '\\002B';
                color: #777;
                font-weight: bold;
                float: right;
                margin-left: 5px;
            
            }
            
            /* Add a background color to the button if it is clicked on (add the .active class with JS), and when you move the mouse over it (hover) */
            .active, .accordion:hover {
                background-color: #ccc;
            }
            
            /* Style the accordion panel. Note: hidden by default */
            .panel {
                padding: 0 0;
                background-color: white;
                display: none;
                overflow: hidden;
            } 
        </style>
        """)

        self.lines.append('</head>')
        self.lines.append('<body>')
        self.lines.append('<h1>{}</h1>'.format(self.name))

    def open_section(self, name):
        self.lines.append('<button class="accordion section">{}</button>'.format(name))
        self.lines.append('<div class="panel">')

    def close_section(self):
        self.lines.append('</div>')

    def add_list(self, name=None, attributes=[]):
        assert len(attributes)

        if not name:
            name = 'list'
        self.lines.append('<button class="accordion">{}</button>'.format(name))

        self.lines.append('<div class="panel">')
        self.lines.append('<table>')
        self.lines.append("""
          <tr>
            <th>Name</th>
            <th>Value</th>
          </tr>
        """)
        for key, value in attributes:
            self.lines.append('<tr>')
            self.lines.append('  <td>{}</td>'.format(key))
            self.lines.append('  <td>{}</td>'.format(value))
            self.lines.append('</tr>')

        self.lines.append('</table>')
        self.lines.append('</div>')

    def add_image(self, name=None, path=None, binary=False):
        if not name:
            name = 'image'
        if name is not None:
            self.lines.append('<button class="accordion">{}</button>'.format(name))

        self.lines.append('<div class="panel">')

        if not binary:
            self.lines.append('<img src="{}" alt="{}" width="100%"/>'.format(path, name))
        else:
            with open(path, "rb") as image_file:
                encoded_string = base64.b64encode(image_file.read())
                self.lines.append('<img src="data:image/gif;base64,{}" alt="{}" width="100%"/>'.format(encoded_string, name))

        self.lines.append('</div>')

    def add_paragraph(self, name=None, text=None):
        if name is not None:
            self.lines.append('<h3>{}</h3>'.format(name))
        if text is not None:
            self.lines.append('<p>{}</p>'.format(text))

    def write(self, filepath):
        lines = list(self.lines)

        lines.append("""
                <script>
                var acc = document.getElementsByClassName("accordion");
                var i;
                
                for (i = 0; i < acc.length; i++) {
                    acc[i].addEventListener("click", function() {
                        this.classList.toggle("active");
                        var panel = this.nextElementSibling;
                        if (panel.style.display === "block") {
                            panel.style.display = "none";
                        } else {
                            panel.style.display = "block";
                        }
                    });
                }
                </script>
                """)

        lines.append('</body>')
        lines.append('</html>')

        with open(filepath, 'w') as f:
            f.writelines(lines)
            f.flush()
            f.close()

        print "Report exported at '{}'".format(filepath)

    def add_image_from_meshes(self, name=None, meshes=[], view_attributes=[], image_width=1000):
        assert len(meshes) == len(view_attributes)
        tempdir = tempfile.gettempdir()
        tempfiles = []
        views = []

        xmin, xmax, ymin, ymax, zmin, zmax = (0, 0, 0, 0, 0, 0)
        i = 0
        for mesh in meshes:
            assert isinstance(mesh, Mesh)
            txmin, txmax, tymin, tymax, tzmin, tzmax = bbox(mesh.vertices)
            xmin, xmax, ymin, ymax, zmin, zmax = min(xmin, txmin), max(xmax, tymax), min(ymin, tymin), max(ymax, tymax), min(zmin, tzmin), max(zmax, tzmax)
            if mesh.filepath is None or not os.path.isfile(mesh.filepath):
                temp = os.path.join(tempdir, 'temp_{}.vtk'.format(i))
                mesh.save(temp)
                tempfiles.append(temp)

            views.append(ParaView.SurfaceView(**dict({
                'vtk_file': mesh.filepath
            }, **view_attributes[i])))
            i = i + 1

        width, height, length = xmax - xmin, ymax - ymin, zmax - zmin
        image_height = int(height / width * image_width)
        camera_angle = 20
        camera_x = xmax + (length / 2. / math.tan(math.radians(camera_angle / 2.))) * 1.15
        camera_y = ymin + (height / 2.)
        camera_z = zmin + (length / 2.)

        temp_image = os.path.join(tempdir, 'temp_image.png')
        tempfiles.append(temp_image)

        ParaView(
            size=(image_width, image_height),
            camera=ParaView.Camera(
                angle=camera_angle,
                position=[-camera_x, camera_y, camera_z],
                focal_point=[xmax, camera_y, camera_z],
            ),
            views=views
        ).save(temp_image)

        self.add_image(name=name, path=temp_image, binary=True)

        for t in tempfiles:
            os.remove(t)
