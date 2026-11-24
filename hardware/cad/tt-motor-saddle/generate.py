import sys,pathlib,json,zipfile
import vtk
import cadquery as cq
HERE=pathlib.Path(__file__).resolve().parent
OUT=HERE/'generated'
OUT.mkdir(parents=True,exist_ok=True)
# mm. Two identical saddles per motor. Nominal TT body 18 wide x20 high.
W=18.6; WALL=3.; FLOOR=3.; LENGTH=10.; H=26.8
PITCH=26.; INSERT_PILOT=4.0; INSERT_DEPTH=5.3
def box(x,y,z,at):return cq.Workplane('XY').box(x,y,z,centered=(True,True,False)).translate(at)
s=box(LENGTH,W+2*WALL,FLOOR,(0,0,0))
for side in [-1,1]:
 s=s.union(box(LENGTH,WALL,H-FLOOR,(0,side*(W/2+WALL/2),FLOOR)))
 s=s.union(box(LENGTH,4.7,6,(0,side*14.65,H-6)))
 # 45 degree support under the projecting flange, printable without support.
 tri=[(side*12.3,16.1),(side*17,20.8),(side*12.3,20.8)]
 s=s.union(cq.Workplane('YZ').polyline(tri).close().extrude(LENGTH).translate((-LENGTH/2,0,0)))
 s=s.cut(cq.Workplane('XY').center(0,side*PITCH/2).circle(INSERT_PILOT/2).extrude(INSERT_DEPTH).translate((0,0,H-INSERT_DEPTH)))
s=s.clean()
assert s.val().isValid() and len(s.solids().vals())==1
cq.exporters.export(s,str(OUT/'TT_saddle_PRINT_4.step'))
cq.exporters.export(s,str(OUT/'TT_saddle_PRINT_4.stl'),tolerance=.05,angularTolerance=.1)
coupon=box(38,14,6,(0,0,0))
for x,d in [(-12,3.8),(0,4.0),(12,4.2)]:coupon=coupon.cut(cq.Workplane('XY').center(x,0).circle(d/2).extrude(INSERT_DEPTH).translate((0,0,6-INSERT_DEPTH)))
cq.exporters.export(coupon,str(OUT/'insert_fit_coupon_PRINT_1.stl'),tolerance=.05,angularTolerance=.1)
cq.exporters.export(coupon,str(OUT/'insert_fit_coupon.step'))
# Local print Z=0 corresponds to assembly Z=19.2, motor centre Y=+/-52.
A=cq.Assembly(name='TT_saddle_nominal_fit')
motor=box(65,18,20,(-28,52,22.5));shaft=cq.Workplane('XZ').circle(2.7).extrude(22).translate((-45,80,32.5))
A.add(motor,name='motor_MEASURE',color=cq.Color(.95,.7,.05));A.add(shaft,name='shaft_MEASURE',color=cq.Color(.7,.7,.7))
report={'units':'mm','print_quantity':4,'motor_body_nominal':[65,18,20],'saddle_size':[10,34,26.8],'insert_pilot_diameter':INSERT_PILOT,'insert_depth':INSERT_DEPTH,'fastener_pitch':PITCH,'solid_valid':s.val().isValid(),'shaft_clearances_mm':{},'motor_overlap_mm3':{}}
for x in [-55,-10]:
 q=s.translate((x,52,19.2));A.add(q,name=f'saddle_{x}',color=cq.Color(.15,.45,.7))
 report['shaft_clearances_mm'][str(x)]=round(q.val().distance(shaft.val()),3)
 inter=q.intersect(motor);report['motor_overlap_mm3'][str(x)]=sum(v.Volume() for v in inter.vals())
 assert report['motor_overlap_mm3'][str(x)]<1e-5
 assert report['shaft_clearances_mm'][str(x)]>2
A.export(str(OUT/'one_motor_fit_reference.step'))
(OUT/'validation.json').write_text(json.dumps(report,indent=2))
# Offscreen preview of the actual printable solid.
r=vtk.vtkRenderer();r.SetBackground(.96,.97,.98)
vs,ts=s.val().tessellate(.1,.1);pts=vtk.vtkPoints();cells=vtk.vtkCellArray()
for v in vs:pts.InsertNextPoint(*v.toTuple())
for tri in ts:
 cells.InsertNextCell(3)
 for j in tri:cells.InsertCellPoint(j)
pd=vtk.vtkPolyData();pd.SetPoints(pts);pd.SetPolys(cells)
m=vtk.vtkPolyDataMapper();m.SetInputData(pd);a=vtk.vtkActor();a.SetMapper(m);a.GetProperty().SetColor(.15,.45,.7);r.AddActor(a)
c=r.GetActiveCamera();c.SetPosition(75,90,65);c.SetFocalPoint(0,0,13);c.SetViewUp(0,0,1);r.ResetCamera()
w=vtk.vtkRenderWindow();w.SetOffScreenRendering(1);w.SetSize(1100,900);w.AddRenderer(r);w.Render()
cap=vtk.vtkWindowToImageFilter();cap.SetInput(w);cap.Update();wr=vtk.vtkPNGWriter();wr.SetFileName(str(OUT/'bracket_preview.png'));wr.SetInputConnection(cap.GetOutputPort());wr.Write();w.Finalize()
print(json.dumps(report,indent=2))
