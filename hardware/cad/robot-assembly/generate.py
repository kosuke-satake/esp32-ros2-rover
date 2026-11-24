import sys, pathlib, json, math, csv, zipfile
import vtk
import cadquery as cq
import ezdxf
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

HERE=pathlib.Path(__file__).resolve().parent
OUT=HERE/'generated'
for d in ['cad/parts','laser','source']: (OUT/d).mkdir(parents=True,exist_ok=True)
T=3.0
# Coordinates: X forward, Y left, Z up; ground Z=0; all mm.
P={}
def rect(w,h): return [(-w/2,-h/2),(w/2,-h/2),(w/2,h/2),(-w/2,h/2)]
def slot(x,y,L,w,angle=0):
    # Exact semicircle vertices represented with DXF bulges (no faceting).
    a=(L-w)/2;r=w/2
    pts=[(-a,-r,0),(a,-r,1),(a,r,0),(-a,r,1)]
    c=math.cos(math.radians(angle));s=math.sin(math.radians(angle))
    return [(x+u*c-v*s,y+u*s+v*c,b) for u,v,b in pts]
def part(name,outline,holes=[],slots=[]):
    P[name]={'outline':outline,'holes':holes,'slots':slots,'rectangles':[]}
def deck(name):
    # 8 mm clipped corners, full 180 x 140 envelope.
    part(name,[(-82,-70),(82,-70),(90,-62),(90,62),(82,70),(-82,70),(-90,62),(-90,-62)],[(x,y,3.4) for x in [-75,75] for y in [-57,57]],[])
deck('01_lower_deck');deck('02_upper_deck')
lo=P['01_lower_deck'];up=P['02_upper_deck']
# Rear strap stays behind nominal axle X=-45; short slots prevent sliding onto it.
lo['slots'] += [(x,y,L,3.5,0) for x,L in [(-55,6),(-10,12)] for y in [-65,-39,39,65]]
# Battery straps, wiring and adjustable caster mounting.
lo['slots'] += [(x,y,18,3.5,0) for x in [-60,-25] for y in [-32,32]]
lo['slots'] += [(x,y,16,3.4,0) for x in [53,77] for y in [-12,12]]
lo['slots'] += [(20,0,24,12,90)]
# ESP carrier attachment, driver/regulator strap lanes, wiring, switch clearance.
up['slots'] += [(-50,y,45,3.4,0) for y in [-20,20]]
up['slots'] += [(x,y,26,3.5,0) for x in [-8,25] for y in [-54,-26]]
up['slots'] += [(x,y,26,3.5,0) for x in [-8,25] for y in [20,46]]
up['slots'] += [(20,0,24,12,90)]
up['rectangles'] += [(-76,0,8.4,15.4)]
# SG90 body opening, ears use small screws and washers, not M3 through servo ears.
up['rectangles'] += [(60,0,24,13)]
up['slots'] += [(60,-14,6,2.2,0),(60,14,6,2.2,0)]
part('03_esp_carrier',rect(72,52),[],[(-25,y,12,3.4,0) for y in [-20,20]]+[(x,y,36,3.2,0) for x in [0] for y in [-12,12]])
part('04_servo_horn_plate',rect(50,56),[(0,0,5),(8,0,2),(-8,0,2)],[])
# Upright located at local plate X=15, with two 8 mm tabs entering base.
P['04_servo_horn_plate']['rectangles'] += [(15,y,3.1,8.2) for y in [-14,14]]
outline=[(-25,0),(-18,0),(-18,-3),(-10,-3),(-10,0),(10,0),(10,-3),(18,-3),(18,0),(25,0),(25,30),(-25,30)]
part('05_sensor_face',outline,[(-13,17,16.4),(13,17,16.4)],[(-19,5,5,2.5,0),(19,5,5,2.5,0)])
part('06_caster_spacer',rect(46,42),[],[(x,y,16,3.4,0) for x in [-12,12] for y in [-12,12]])
part('07_kerf_coupon',rect(76,32),[(27,0,3.4)],[(x,0,12,w,90) for x,w in zip([-27,-16,-5,6],[2.9,3.0,3.1,3.2])])

def wire_poly(pts):
    return cq.Workplane('XY').polyline(pts).close()
def solid(p):
    s=wire_poly(p['outline']).extrude(T)
    for x,y,d in p['holes']:s=s.cut(cq.Workplane('XY').center(x,y).circle(d/2).extrude(T))
    for x,y,L,w,a in p['slots']:s=s.cut(cq.Workplane('XY').center(x,y).slot2D(L,w,a).extrude(T))
    for x,y,w,h in p['rectangles']:s=s.cut(cq.Workplane('XY').center(x,y).rect(w,h).extrude(T))
    return s
def write_part(msp,p,dx=0,dy=0):
    msp.add_lwpolyline([(x+dx,y+dy) for x,y in p['outline']],close=True,dxfattribs={'layer':'CUT'})
    for x,y,d in p['holes']:msp.add_circle((x+dx,y+dy),d/2,dxfattribs={'layer':'CUT'})
    for x,y,L,w,a in p['slots']:msp.add_lwpolyline([(u+dx,v+dy,b) for u,v,b in slot(x,y,L,w,a)],format='xyb',close=True,dxfattribs={'layer':'CUT'})
    for x,y,w,h in p['rectangles']:msp.add_lwpolyline([(u+x+dx,v+y+dy) for u,v in rect(w,h)],close=True,dxfattribs={'layer':'CUT'})
def doc():
    d=ezdxf.new('R2010');d.units=4;d.layers.new('CUT',dxfattribs={'color':1});return d
S={}
for n,p in P.items():
    s=solid(p); assert s.val().isValid();S[n]=s
    cq.exporters.export(s,str(OUT/'cad/parts'/f'{n}.step'))
    d=doc();write_part(d.modelspace(),p);d.saveas(OUT/'laser'/f'{n}.dxf')
# One cut of each, including one optional spacer; repeated spacers cut separately as required.
layout={'01_lower_deck':(100,80),'02_upper_deck':(100,235),'03_esp_carrier':(240,40),'04_servo_horn_plate':(240,110),'05_sensor_face':(240,165),'06_caster_spacer':(240,225),'07_kerf_coupon':(245,280)}
d=doc()
for n,(x,y) in layout.items():write_part(d.modelspace(),P[n],x,y)
d.saveas(OUT/'laser'/'ALL_PARTS_300x320_nominal.dxf')

A=cq.Assembly(name='ESP32_S3_robot_nominal')
items=[]
def add(n,s,color,kind='reference'):
    assert s.val().isValid(),n
    A.add(s,name=n,color=cq.Color(*color));items.append((n,s,color,kind))
wood=(0.72,0.49,0.25);blue=(0.1,0.32,0.7);green=(0.1,0.5,0.28);black=(0.15,0.16,0.18)
add('01_lower_deck',S['01_lower_deck'].translate((0,0,46)),wood,'wood')
add('02_upper_deck',S['02_upper_deck'].translate((0,0,79)),wood,'wood')
add('03_esp_carrier',S['03_esp_carrier'].translate((-50,0,88)),wood,'wood')
add('04_servo_horn_plate',S['04_servo_horn_plate'].translate((65,0,104)),wood,'wood')
# local u -> global Y; local v -> global Z; thickness -> global X.
face=S['05_sensor_face'].rotate((0,0,0),(1,1,1),120).translate((78.5,0,107))
add('05_sensor_face',face,wood,'wood')
def box(w,h,z,x,y,b):return cq.Workplane('XY').box(w,h,z,centered=(True,True,False)).translate((x,y,b))
def cyl(r,h,x,y,z):return cq.Workplane('XY').circle(r).extrude(h).translate((x,y,z))
for x in [-75,75]:
 for y in [-57,57]:add(f'M3_standoff_{x}_{y}',cyl(3,30,x,y,49).cut(cyl(1.5,30,x,y,49)),(.65,.65,.68),'hardware')
for y in [-20,20]:add(f'carrier_spacer_{y}',cyl(3,6,-70,y,82).cut(cyl(1.7,6,-70,y,82)),(.7,.7,.7),'hardware')
for side in [-1,1]:
    add(f'TT_motor_{side}_MEASURE',box(65,18,20,-28,side*52,22.5),(0.95,.72,.05))
    wheel=cq.Workplane('XZ').circle(32.5).extrude(26).translate((-45,side*90+13,32.5))
    add(f'wheel_{side}_65x26_MEASURE',wheel,black)
    shaft=cq.Workplane('XZ').circle(2.7).extrude(22).translate((-45,side*69+11,32.5))
    add(f'axle_{side}_reference',shaft,(.7,.7,.7))
    for x in [-55,-10]:
        tie=box(2.5,27.5,28.5,x,side*52,21.5).cut(box(4.5,25.5,26.5,x,side*52,22.5))
        add(f'motor_strap_{side}_{x}_route_reference',tie,(.25,.25,.28))
add('battery_4AA_60x58x19_MEASURE',box(60,58,19,-44,0,49),black)
add('ESP32_S3_64x28_MEASURE',box(64,28,1.6,-50,0,96),green)
add('ESP32_headers_keepout_MEASURE',box(60,26,5,-50,0,91),black)
add('TB6612_22x20_MEASURE',box(22,20,8,-12,-40,86),green)
add('regulator_35x22_MEASURE',box(35,22,12,25,33,86),blue)
add('switch_MEASURE',box(8,15,12,-76,0,75),black)
add('SG90_body_MEASURE',box(23,12.2,22.8,60,0,59.2),blue)
for y in [-14,14]:add(f'SG90_ear_{y}_MEASURE',box(12,4,2,60,y,82),blue)
add('SG90_tower_MEASURE',cyl(5.8,8,65,0,84),blue)
add('servo_horn_reference',box(22,6,2,65,0,92),(.9,.9,.9))
for x in [57,73]:add(f'horn_spacer_{x}_MEASURE',cyl(2,10,x,0,94).cut(cyl(1,10,x,0,94)),(.7,.7,.7),'hardware')
# HC PCB behind plywood face, cans pass through two cutouts.
add('HC_SR04_PCB_MEASURE',box(1.6,45,20,77.7,0,114),green)
for y in [-13,13]:add(f'HC_transducer_{y}',cq.Workplane('YZ').circle(8).extrude(12).translate((78.5,y,124)),(.7,.72,.75))
# 25 mm caster overall, 21 mm adjustable riser required to base underside at 46.
add('caster_mount_MEASURE',box(32,30,3,65,0,22),(.6,.6,.62))
add('caster_housing_envelope',cyl(12,10,65,0,12),(.6,.6,.62))
add('caster_ball',cq.Workplane('XY').sphere(8).translate((65,0,8)),(.75,.75,.78))
for x in [53,77]:
 for y in [-12,12]:add(f'caster_riser_{x}_{y}_ADJUST',cyl(3,21,x,y,25).cut(cyl(1.7,21,x,y,25)),(.7,.7,.7),'hardware')
A.export(str(OUT/'cad'/'robot_assembly.step'))
# Export positioned references separately, alongside flat manufacturing parts.
for n,s,c,k in items:
 if k!='wood':cq.exporters.export(s,str(OUT/'cad/parts'/f'{n}.step'))

# Verify STEP round trip, DXF audit, all solids, wood-to-wood intersections.
report={'units':'mm','thickness':T,'parts':len(items),'solid_validity':all(s.val().isValid() for _,s,_,_ in items),'dxf_audit':{},'wood_intersections_mm3':{},'critical_clearances_mm':{'wheel_to_deck_lateral':7,'battery_to_upper_underside':11,'motor_to_lower_underside':3.5,'caster_riser_nominal':21,'deck_gap':30}}
for f in (OUT/'laser').glob('*.dxf'):
    dd=ezdxf.readfile(f);au=dd.audit();report['dxf_audit'][f.name]={'errors':len(au.errors),'fixes':len(au.fixes),'entities':len(dd.modelspace())}
re=cq.importers.importStep(str(OUT/'cad'/'robot_assembly.step'));report['step_roundtrip_solids']=len(re.solids().vals())
woods=[i for i in items if i[3]=='wood']
for i,(n,s,_,_) in enumerate(woods):
 for n2,s2,_,_ in woods[i+1:]:
    v=s.intersect(s2).val().Volume() if s.intersect(s2).vals() else 0
    if v>1e-5:report['wood_intersections_mm3'][n+' / '+n2]=v
report['wood_reference_intersections_mm3']={}
for n,s,_,_ in woods:
 for n2,s2,_,k in items:
    if k!='reference':continue
    inter=s.intersect(s2)
    v=sum(v.Volume() for v in inter.vals())
    if v>1e-4:report['wood_reference_intersections_mm3'][n+' / '+n2]=v
report['motor_strap_to_axle_clearance_mm']={}
for n,s,_,_ in items:
 if n.startswith('motor_strap'):
  for n2,s2,_,_ in items:
   if n2.startswith('axle_'):
    report['motor_strap_to_axle_clearance_mm'][n+' / '+n2]=round(s.val().distance(s2.val()),3)
(OUT/'validation.json').write_text(json.dumps(report,indent=2))

# Physical geometry preview, using tessellation of the exported CAD objects.
fig=plt.figure(figsize=(12,9));ax=fig.add_subplot(111,projection='3d')
allpolys=[];allcolors=[]
for n,s,c,k in items:
 vs,ts=s.val().tessellate(.8,.25);verts=[v.toTuple() for v in vs]
 polys=[[verts[j] for j in tri] for tri in ts]
 allpolys.extend(polys);allcolors.extend([c]*len(polys))
ax.add_collection3d(Poly3DCollection(allpolys,facecolors=allcolors,edgecolors='none',alpha=1,zsort='average'))
ax.set(xlim=(-100,110),ylim=(-110,110),zlim=(0,145),xlabel='X forward (mm)',ylabel='Y (mm)',zlabel='Z (mm)')
ax.set_box_aspect((210,220,145));ax.view_init(25,35);ax.set_title('ESP32-S3 robot | nominal assembly\n180 x 140 mm decks; component envelopes require measurement',pad=22)
fig.tight_layout();fig.savefig(OUT/'assembly_preview.png',dpi=160);plt.close(fig)
# Use a depth-buffered render for the final 3D preview (avoids painter sorting artefacts).
renderer=vtk.vtkRenderer();renderer.SetBackground(.96,.97,.98)
for n,s,c,k in items:
 vs,ts=s.val().tessellate(.5,.15);points=vtk.vtkPoints();cells=vtk.vtkCellArray()
 for v in vs:points.InsertNextPoint(*v.toTuple())
 for tri in ts:
  cells.InsertNextCell(3)
  for idx in tri:cells.InsertCellPoint(idx)
 data=vtk.vtkPolyData();data.SetPoints(points);data.SetPolys(cells)
 normals=vtk.vtkPolyDataNormals();normals.SetInputData(data);normals.ComputePointNormalsOn();normals.SplittingOn()
 mapper=vtk.vtkPolyDataMapper();mapper.SetInputConnection(normals.GetOutputPort())
 actor=vtk.vtkActor();actor.SetMapper(mapper);actor.GetProperty().SetColor(*c);renderer.AddActor(actor)
camera=renderer.GetActiveCamera();camera.SetPosition(320,300,280);camera.SetFocalPoint(0,0,65);camera.SetViewUp(0,0,1);renderer.ResetCamera()
window=vtk.vtkRenderWindow();window.SetOffScreenRendering(1);window.AddRenderer(renderer);window.SetSize(1600,1200);window.SetMultiSamples(4);window.Render()
capture=vtk.vtkWindowToImageFilter();capture.SetInput(window);capture.Update()
writer=vtk.vtkPNGWriter();writer.SetFileName(str(OUT/'assembly_preview.png'));writer.SetInputConnection(capture.GetOutputPort());writer.Write();window.Finalize()
fig,ax=plt.subplots(figsize=(9,10))
for n,(dx,dy) in layout.items():
 p=P[n];pts=p['outline']+[p['outline'][0]];ax.plot([x+dx for x,y in pts],[y+dy for x,y in pts],color='brown')
 for x,y,dia in p['holes']:ax.add_patch(plt.Circle((x+dx,y+dy),dia/2,fill=False,color='red'))
 for x,y,w,h in p['rectangles']:ax.add_patch(plt.Rectangle((x+dx-w/2,y+dy-h/2),w,h,fill=False,color='red'))
 for x,y,L,w,a in p['slots']:
    pts=[];ang=math.radians(a);r=w/2;h=(L-w)/2
    for cx,start in [(h,-90),(-h,90)]:
     for j in range(25):
      t=math.radians(start+j*180/24);u=cx+r*math.cos(t);v=r*math.sin(t)
      pts.append((x+dx+u*math.cos(ang)-v*math.sin(ang),y+dy+u*math.sin(ang)+v*math.cos(ang)))
    pts.append(pts[0]);ax.plot([u for u,v in pts],[v for u,v in pts],color='red',linewidth=.6)
 ax.text(dx,dy+max(y for x,y in p['outline'])+3,n,ha='center',fontsize=8)
ax.set_aspect('equal');ax.set(xlim=(0,300),ylim=(0,320),xlabel='mm',ylabel='mm',title='300 x 320 mm nominal layout | CUT only in DXF');fig.tight_layout();fig.savefig(OUT/'laser_layout_preview.png',dpi=160);plt.close(fig)
(OUT/'source'/'parameters.json').write_text(json.dumps({'thickness':T,'parts':P,'layout':layout},indent=2))
print(json.dumps(report,indent=2))
