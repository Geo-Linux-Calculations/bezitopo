/******************************************************/
/*                                                    */
/* bezitopo.cpp - main program                        */
/*                                                    */
/******************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include "point.h"
#include "cogo.h"
#include "bezitopo.h"
#include "test.h"
#include "tin.h"
#include "measure.h"
#include "pnezd.h"
#include "angle.h"
#include "pointlist.h"
#include "vcurve.h"
#include "raster.h"
#include "ps.h"
#include "icommon.h"
#include "mkpoint.h"
#include "closure.h"
#include "cvtmeas.h"
#include "contour.h"

using namespace std;

bool cont=true;
document doc;
string savefilename;

void indpark(string args)
/* Concrete rectangle with apparent overlap: elevation 203.7
 * Hooklike appendage a few meters west of concrete rectangle: 203.8
 * Outline that appears wrong, but isn't: 204.2
 * Spurious semicircle: 204.8
 * Apparent loop: 206.3
 * Spiralarc clear across the scene: 207.7
 * 
 * On 207.7: the contour beginning at (-7.574323437274727,242.760955397681) should
 * continue from (293.0923474548852,35.76812401806803) to (293.17543184146035,
 * 34.75709173391407), but instead jumps back to the starting point.
 * 
 * On 204.8: the contour beginning at (49.294836688628344,78.90549573888802) has
 * four points; one of the spiralarcs has delta 883276262 (148.07°) and delta2 -6.9°
 * and happens to touch another contour at the same elevation, so the program didn't
 * notice that it should break it. Tried to fix it by checking that the spiralarc
 * is entirely in the triangle containing the midpoint of the segment (it isn't,
 * of course) and if so, applying a tolerance of 0 to force it to split. The problem
 * is that the cross segment at the split point does not intercept 204.8. Setting
 * BENDLIMIT to DEG120 fixes it.
 * 
 * Triangle (956 1112 430) is undivided, but contours passing through it southeast
 * of point 1112 are bent enough that tracing results in backtracking.
 */
{
  int i,j,itype;
  double w,e,s,n;
  ofstream ofile("IndependencePark.bez");
  criteria crit;
  criterion crit1;
  doc.offset=xyz(0,0,0);
  setfoot(USSURVEY);
  set_length_unit(FOOT+DEC2);
  if (doc.readpnezd("topo0.asc")<0)
    doc.readpnezd("../topo0.asc");
  crit1.str="";
  crit1.istopo=true;
  crit.push_back(crit1);
  crit1.str="FH";
  crit1.istopo=false; // The point labeled FH has a nonsensical elevation and must be removed.
  crit.push_back(crit1);
  doc.copytopopoints(crit);
  doc.changeOffset(xyz(443392,164096,208));
  doc.pl[1].maketin("bezitopo.ps");
  doc.pl[1].makegrad(0.15);
  doc.pl[1].maketriangles();
  doc.pl[1].setgradient(false);
  doc.pl[1].makeqindex();
  doc.pl[1].findcriticalpts();
  doc.pl[1].addperimeter();
  w=doc.pl[1].dirbound(degtobin(0));
  s=doc.pl[1].dirbound(degtobin(90));
  e=-doc.pl[1].dirbound(degtobin(180));
  n=-doc.pl[1].dirbound(degtobin(270));
  cout<<"Writing topo with curved triangles"<<endl;
  rasterdraw(doc.pl[1],xy((e+w)/2,(n+s)/2),e-w,n-s,10,0,10,"IndependencePark.ppm");
  /* The first (lowest) contour in which a NaN control point is drawn on a
   * polyspiral contour segment whose ends are real is the following:
   * 302.559,316.867 302.398,316.802 302.488,316.551 302.557,316.826 302.517,316.637
   * Draw the surrounding region to find out where it is. It is next to the
   * box culvert on the northwest side of the field. The surface is grainy.
   */
  rasterdraw(doc.pl[1],xy(443302.5,164316.7)-(xy)doc.offset,0.25,0.35,1000,0,10,"IPmicro.ppm");
  rasterdraw(doc.pl[1],xy(443302.5,164316.7)-(xy)doc.offset,7,7,100,0,10,"IPmini.ppm");
  roughcontours(doc.pl[1],0.1);
  doc.pl[1].removeperimeter();
  smoothcontours(doc.pl[1],0.1);
  psopen("IndependencePark.ps");
  psprolog();
  startpage();
  setscale(-n,w,-s,e,DEG90);
  setcolor(0,0.6,0.6);
  for (i=0;i<doc.pl[1].edges.size();i++)
    spline(doc.pl[1].edges[i].getsegment().approx3d(1));
  setcolor(0,1,1);
  for (i=0;i<doc.pl[1].triangles.size();i++)
    for (j=0;j<doc.pl[1].triangles[i].subdiv.size();j++)
      spline(doc.pl[1].triangles[i].subdiv[j].approx3d(1));
  for (i=0;i<doc.pl[1].contours.size();i++)
  {
    if (i<0 && doc.pl[1].contours[i].getElevation()>doc.pl[1].contours[i-1].getElevation())
    {
      endpage();
      startpage();
      setscale(-n,w,-s,e,DEG90);
    }
    switch (lrint(doc.pl[1].contours[i].getElevation()/0.1)%10)
    {
      case 0:
	setcolor(1,0,0);
	break;
      case 5:
	setcolor(0,0,1);
	break;
      default:
	setcolor(0,0,0);
    }
    spline(doc.pl[1].contours[i].approx3d(0.1));
  }
  endpage();
  pstrailer();
  psclose();
  doc.writeXml(ofile);
  doc.pl[1].setgradient(true);
  cout<<"Writing topo with flat triangles"<<endl;
  rasterdraw(doc.pl[1],xy((e+w)/2,(n+s)/2),e-w,n-s,10,0,10,"IndependencePark-flat.ppm");
}

vector<command> commands;

void readpoints(string args)
// just pnezd for now
{
  doc.readpnezd(trim(args),false);
}

void writepoints(string args)
// just pnezd for now
{
  doc.writepnezd(trim(args));
}

void maketin_i(string args)
{
  int error=0;
  criteria crit;
  criterion crit1;
  crit1.str="";
  crit1.istopo=true;
  crit.push_back(crit1); // will later make a point-selection command
  doc.copytopopoints(crit);
  try
  {
    doc.pl[1].maketin("maketin.ps");
  }
  catch(int e)
  {
    error=e;
  }
  switch (error)
  {
    case notri:
      cout<<"Less than three points selected.\nPlease load a coordinate file or make points."<<endl;
      break;
    case samepnts:
      cout<<"Two points have the same x and y coordinates. Deselect one."<<endl;
      break;
    case flattri:
      cout<<"Couldn't make a TIN. Looks like all the points are collinear."<<endl;
      break;
    default:
      cout<<"Successfully made TIN."<<endl;
      doc.pl[1].makegrad(0.15);
      doc.pl[1].maketriangles();
      doc.pl[1].setgradient(false);
      doc.pl[1].makeqindex();
  }
}

void drawtin_i(string args)
{
  double w,e,s,n;
  if (doc.pl[1].edges.size())
  {
    w=doc.pl[1].dirbound(degtobin(0));
    s=doc.pl[1].dirbound(degtobin(90));
    e=-doc.pl[1].dirbound(degtobin(180));
    n=-doc.pl[1].dirbound(degtobin(270));
    psopen(trim(args).c_str());
    psprolog();
    setscale(w,s,n,e);
    startpage();
    doc.pl[1].dumpedges_ps(false);
    endpage();
    pstrailer();
    psclose();
  }
  else
    cout<<"No TIN present. Please make a TIN first."<<endl;
}

void trin_i(string args)
{
  triangle *tri;
  xy pnt;
  if (doc.pl[1].edges.size())
  {
    pnt=parsexy(args);
    tri=doc.pl[1].qinx.findt(pnt);
    if (tri)
      cout<<doc.pl[1].revpoints[tri->a]<<' '<<doc.pl[1].revpoints[tri->b]<<' '<<doc.pl[1].revpoints[tri->c]<<endl;
    else
      cout<<"Not in a triangle"<<endl;
  }
  else
    cout<<"No TIN present. Please make a TIN first."<<endl;
}

void rasterdraw_i(string args)
{
  double w,e,s,n;
  w=doc.pl[1].dirbound(degtobin(0));
  s=doc.pl[1].dirbound(degtobin(90));
  e=-doc.pl[1].dirbound(degtobin(180));
  n=-doc.pl[1].dirbound(degtobin(270));
  rasterdraw(doc.pl[1],xy((e+w)/2,(n+s)/2),e-w,n-s,10,0,10,trim(args));
}

void contourdraw_i(string args)
{
  string contervalstr;
  double conterval;
  double w,e,s,n;
  int i,j;
  contervalstr=firstarg(args);
  conterval=parse_length(contervalstr);
  if (conterval>5e-6 && conterval<1e5)
    if (doc.pl[1].edges.size())
    {
      doc.pl[1].findcriticalpts();
      doc.pl[1].addperimeter();
      roughcontours(doc.pl[1],conterval);
      doc.pl[1].removeperimeter();
      smoothcontours(doc.pl[1],conterval,true);
      w=doc.pl[1].dirbound(degtobin(0));
      s=doc.pl[1].dirbound(degtobin(90));
      e=-doc.pl[1].dirbound(degtobin(180));
      n=-doc.pl[1].dirbound(degtobin(270));
      psopen(args.c_str());
      psprolog();
      startpage();
      setscale(w,s,e,n,0);
      setcolor(0,0.6,0.6);
      for (i=0;i<doc.pl[1].edges.size();i++)
	spline(doc.pl[1].edges[i].getsegment().approx3d(1));
      setcolor(0,1,1);
      for (i=0;i<doc.pl[1].triangles.size();i++)
	for (j=0;j<doc.pl[1].triangles[i].subdiv.size();j++)
	  spline(doc.pl[1].triangles[i].subdiv[j].approx3d(1));
      for (i=0;i<doc.pl[1].contours.size();i++)
      {
	switch (lrint(doc.pl[1].contours[i].getElevation()/conterval)%10)
	{
	  case 0:
	    setcolor(1,0,0);
	    break;
	  case 5:
	    setcolor(0,0,1);
	    break;
	  default:
	    setcolor(0,0,0);
	}
	spline(doc.pl[1].contours[i].approx3d(0.1));
      }
      endpage();
      pstrailer();
      psclose();
    }
    else
      cout<<"No TIN present. Please make a TIN first."<<endl;
  else
    cout<<"Contour interval should be between 5 µm and 10 km"<<endl;
}

void save_i(string args)
{
  ofstream ofile;
  args=trim(args);
  if (args.length())
    savefilename=args;
  if (savefilename.length())
  {
    ofile.open(savefilename);
    doc.writeXml(ofile);
    ofile.close();
  }
  else
    cout<<"No filename specified"<<endl;
}

void help(string args)
{
  int i;
  for (i=0;i<commands.size();i++)
  {
    cout<<commands[i].word<<"  "<<commands[i].desc<<endl;
  }
}

void exit(string args)
{
  cont=false;
}

int main(int argc, char *argv[])
{
  int i,cmd;
  size_t chpos;
  string cmdline,cmdword,cmdargs;
  commands.push_back(command("indpark",indpark,"Process the Independence Park topo (topo0.asc)"));
  commands.push_back(command("closure",closure_i,"Check closure of a lot"));
  commands.push_back(command("mkpoint",mkpoint_i,"Make new points"));
  commands.push_back(command("setfoot",setfoot_i,"Set foot unit: int'l, US, Indian"));
  commands.push_back(command("setlunit",setlengthunit_i,"Set length unit: m, ft, ch"));
  commands.push_back(command("cvtmeas",cvtmeas_i,"Convert measurements"));
  commands.push_back(command("read",readpoints,"Read coordinate file in PNEZD format: filename"));
  commands.push_back(command("write",writepoints,"Write coordinate file in PNEZD format: filename"));
  commands.push_back(command("save",save_i,"Write scene file: filename.bez"));
  commands.push_back(command("maketin",maketin_i,"Make triangulated irregular network"));
  commands.push_back(command("drawtin",drawtin_i,"Draw TIN: filename.ps"));
  commands.push_back(command("raster",rasterdraw_i,"Draw raster topo: filename.ppm"));
  commands.push_back(command("contour",contourdraw_i,"Draw contour topo: interval filename.ps"));
  commands.push_back(command("trin",trin_i,"Find what triangle a point is in: x,y"));
  commands.push_back(command("help",help,"List commands"));
  commands.push_back(command("exit",exit,"Exit the program"));
  doc.pl.resize(1);
  while (cont)
  {
    cout<<"? ";
    cout.flush();
    getline(cin,cmdline);
    chpos=cmdline.find_first_of(' '); // split the string at the first space
    if (chpos>cmdline.length())
      chpos=cmdline.length();
    cmdword=cmdline.substr(0,chpos);
    cmdargs=cmdline.substr(chpos);
    for (cmd=-1,i=0;i<commands.size();i++)
      if (commands[i].word==cmdword)
	cmd=i;
    if (cmd>=0)
      commands[cmd].fun(cmdargs);
    else
      cout<<"Unrecognized command, type \"help\" for a list of commands"<<endl;
  }
  return EXIT_SUCCESS;
}
