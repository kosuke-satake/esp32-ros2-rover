import sys,pathlib,math,json,shutil,zipfile
import vtk
import cadquery as cq
HERE=pathlib.Path(__file__).resolve().parent
OUT=HERE/'generated'
OUT.mkdir(parents=True,exist_ok=True)
OD=65.;RIM_ID=54.;WIDTH=26.;HUB_D=20.;HUB_LENGTH=40.;SPOKES=6;WEB=6.
SHAFT_D=5.4;SHAFT_FLATS=3.6;FIT_ALLOWANCE=.15;BORE_DEPTH=12.
def cylinder(r,h,z=0):return cq.Workplane('XY').circle(r).extrude(h).translate((0,0,z))
def shaft_shape(clearance,height):
 return cylinder((SHAFT_D+clearance)/2,height).intersect(cq.Workplane('XY').rect(SHAFT_D+clearance+2,SHAFT_FLATS+clearance).extrude(height))
def p(r,a):return (r*math.cos(math.radians(a)),r*math.sin(math.radians(a)))
# Rim and six-spoke web. True arc-shaped windows go all the way through the web.
rim=cq.Workplane('XY').circle(OD/2).circle(RIM_ID/2).extrude(WIDTH)
web=cylinder(RIM_ID/2,WEB)
for i in range(SPOKES):
 a=i*360/SPOKES+10;b=(i+1)*360/SPOKES-10
 window=cq.Workplane('XY').moveTo(*p(12,a)).lineTo(*p(27,a)).threePointArc(p(27,(a+b)/2),p(27,b)).lineTo(*p(12,b)).threePointArc(p(12,(a+b)/2),p(12,a)).close().extrude(WEB)
 web=web.cut(window)
# Slim inboard neck clears the previously designed saddle and tie routes.
hub=cylinder(10,30).union(cq.Workplane('XY').add(cq.Solid.makeCone(10,4.5,6)).translate((0,0,30))).union(cylinder(4.5,4,36))
wheel=rim.union(web).union(hub)
# Recessed lateral tread bands preserve continuous 65 mm contact ribs.
for z in [6,12,18]:wheel=wheel.cut(cq.Workplane('XY').circle(33).circle(31.8).extrude(1.4).translate((0,0,z)))
wheel=wheel.cut(shaft_shape(FIT_ALLOWANCE,BORE_DEPTH).translate((0,0,HUB_LENGTH-BORE_DEPTH)))
wheel=wheel.clean()
assert wheel.val().isValid() and len(wheel.solids().vals())==1
cq.exporters.export(wheel,str(OUT/'wheel_65mm_6spoke_PRINT_2.step'))
cq.exporters.export(wheel,str(OUT/'wheel_65mm_6spoke_PRINT_2.stl'),tolerance=.04,angularTolerance=.08)
coupons=[]
for label,clear in [('tight',0),('nominal',.15),('loose',.30)]:
 q=cylinder(4.5,12).cut(shaft_shape(clear,12)).clean()
 cq.exporters.export(q,str(OUT/f'axle_fit_{label}.stl'),tolerance=.04,angularTolerance=.08)
 coupons.append({'file':f'axle_fit_{label}.stl','major_diameter':SHAFT_D+clear,'across_flats':SHAFT_FLATS+clear})

base=cq.importers.importStep(str(HERE.parent/'chassis-base/chassis-base.step')).translate((0,0,46)) if (HERE.parent/'chassis-base/chassis-base.step').exists() else cq.Workplane('XY').box(180,140,3,centered=(True,True,False)).translate((0,0,46))
upper=cq.Workplane('XY').box(180,140,3,centered=(True,True,False)).translate((0,0,79))
A=cq.Assembly(name='spoked_wheel_chassis_clearance');A.add(base,name='lower_deck',color=cq.Color(.72,.49,.25));A.add(upper,name='upper_deck_envelope',color=cq.Color(.72,.49,.25))
items=[('lower_deck',base,(.72,.49,.25)),('upper_deck',upper,(.72,.49,.25))]
report={'units':'mm','wheel_OD':OD,'rim_width':WIDTH,'hub_overall_length':HUB_LENGTH,'hub_inboard_projection':14,'window_count':6,'print_quantity':2,'nominal_shaft':[SHAFT_D,SHAFT_FLATS],'bore':[SHAFT_D+FIT_ALLOWANCE,SHAFT_FLATS+FIT_ALLOWANCE],'fit_coupons':coupons,'fit_status':'Nominal double-flat shaft; measure and test actual motor','checks':{}}
for side in [-1,1]:
 w=wheel.rotate((0,0,0),(1,0,0),side*90).translate((-45,side*103,32.5))
 shaft=shaft_shape(0,10).rotate((0,0,0),(1,0,0),side*90).translate((-45,side*71,32.5))
 A.add(w,name=f'wheel_{side}',color=cq.Color(.12,.28,.4));A.add(shaft,name=f'shaft_nominal_{side}',color=cq.Color(.7,.7,.7));items.extend([(f'wheel_{side}',w,(.12,.28,.4)),(f'shaft_{side}',shaft,(.7,.7,.7))])
 for n,s in [('lower_deck',base),('upper_deck',upper),('shaft',shaft)]:
  inter=w.intersect(s);v=sum(t.Volume() for t in inter.vals());report['checks'][f'{side}_{n}']={'overlap_mm3':round(v,6),'minimum_distance_mm':round(w.val().distance(s.val()),3)}
  assert v<1e-5
 saddle_path=(HERE.parent/'tt-motor-saddle/tt-motor-saddle.step')
 if saddle_path.exists():
  for x in [-56,-10]:
   saddle=cq.importers.importStep(str(saddle_path)).translate((x,side*52,19.2));inter=w.intersect(saddle);v=sum(t.Volume() for t in inter.vals())
   report['checks'][f'{side}_saddle_{x}']={'overlap_mm3':round(v,6),'minimum_distance_mm':round(w.val().distance(saddle.val()),3)}
   assert v<1e-5
report['rim_to_deck_lateral_gap_mm']=7.;report['hub_envelope_vertical_gap_mm']=3.5;report['shaft_engagement_nominal_mm']=8.;report['rear_printed_saddle_X_mm']=-56
A.export(str(OUT/'chassis_wheel_clearance.step'))
# Verify triangulated exports are closed and manifold, and STEP round trip is one solid.
report['stl_checks']={}
for f in OUT.glob('*.stl'):
 rd=vtk.vtkSTLReader();rd.SetFileName(str(f));rd.Update();cl=vtk.vtkCleanPolyData();cl.SetInputConnection(rd.GetOutputPort());cl.Update()
 ed=vtk.vtkFeatureEdges();ed.SetInputConnection(cl.GetOutputPort());ed.BoundaryEdgesOn();ed.NonManifoldEdgesOn();ed.FeatureEdgesOff();ed.ManifoldEdgesOff();ed.Update()
 count=ed.GetOutput().GetNumberOfCells();assert count==0
 report['stl_checks'][f.name]={'boundary_or_nonmanifold_edges':count,'triangles':rd.GetOutput().GetNumberOfCells()}
assert len(cq.importers.importStep(str(OUT/'wheel_65mm_6spoke_PRINT_2.step')).solids().vals())==1
(OUT/'validation.json').write_text(json.dumps(report,indent=2))

def render(objects,path,camera,focal):
 r=vtk.vtkRenderer();r.SetBackground(.95,.97,.98)
 for n,s,col in objects:
  vs,ts=s.val().tessellate(.25,.12);pts=vtk.vtkPoints();cells=vtk.vtkCellArray()
  for v in vs:pts.InsertNextPoint(*v.toTuple())
  for tri in ts:
   cells.InsertNextCell(3)
   for j in tri:cells.InsertCellPoint(j)
  pd=vtk.vtkPolyData();pd.SetPoints(pts);pd.SetPolys(cells);norm=vtk.vtkPolyDataNormals();norm.SetInputData(pd);norm.SplittingOn();norm.Update();m=vtk.vtkPolyDataMapper();m.SetInputConnection(norm.GetOutputPort());a=vtk.vtkActor();a.SetMapper(m);a.GetProperty().SetColor(*col);r.AddActor(a)
 c=r.GetActiveCamera();c.SetPosition(*camera);c.SetFocalPoint(*focal);c.SetViewUp(0,0,1);r.ResetCamera();win=vtk.vtkRenderWindow();win.SetOffScreenRendering(1);win.AddRenderer(r);win.SetSize(1400,1100);win.SetMultiSamples(4);win.Render();cap=vtk.vtkWindowToImageFilter();cap.SetInput(win);cap.Update();wr=vtk.vtkPNGWriter();wr.SetFileName(str(path));wr.SetInputConnection(cap.GetOutputPort());wr.Write();win.Finalize()
# View through the open outer face, with print-bed face pointing toward viewer.
render([('wheel',wheel,(.12,.28,.4))],OUT/'wheel_preview.png',(90,-110,-85),(0,0,15))
render(items,OUT/'chassis_clearance_preview.png',(280,330,230),(0,0,45))
print(json.dumps(report,indent=2))
