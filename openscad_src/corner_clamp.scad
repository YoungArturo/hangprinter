include <parameters.scad>
use <util.scad>

corner_clamp();
module corner_clamp(){
  wall_th = Wall_th+0.2; // A tad thicker since flex is very unwanted in this piece
  rad_b = 4;
  a = 13;
  b = Fat_beam_width+wall_th+2-a/2;
  l0 = 40;
  l1 = (Fat_beam_width+2*wall_th)*2*sqrt(3);
  d_hole_l = l0/2+2;

  // Channel to guide D-line and stiffen up corner
  channel_l = l1/sqrt(3)-2*rad_b-1.3-2.5/2;
  channel_r1 = 1;
  channel_r2 = 3;

  difference(){
    union(){
      difference(){
        union(){
          translate([-l1/2,-l1/sqrt(12),0])
          linear_extrude(height=wall_th)
            polygon(points = my_rounded_eqtri(l1,rad_b,5));
          for(k=[0,1])
            mirror([k,0,0])
              rotate([0,0,-30]){
                translate([-(Fat_beam_width+wall_th)+1, -l0+wall_th*sqrt(3), 0]){
                  one_rounded_cube4([Fat_beam_width+wall_th-1, l0+a, Fat_beam_width+2*wall_th],
                      2, $fn=4*4);
                  translate([-wall_th-1,0,0])
                    rounded_cube2([2*wall_th+1, l0+a, wall_th],2,$fn=4*4);
                  }
                }
          translate([0,-sqrt(2)/sqrt(5),wall_th])
            rotate([0,0,-90-45])
            inner_round_corner(1,Fat_beam_width+wall_th,120,1.0, $fn=4*8);
        } // end union
        zip_fr_edg = 8;
        for(k=[0,1])
          mirror([k,0,0])
            rotate([0,0,-30]){
              translate([-Fat_beam_width-wall_th, -l0+4, wall_th]){
                cube([Fat_beam_width, l0+a+2, Fat_beam_width+20]);
                translate([-wall_th,0,-wall_th]){
                  opening_top(exclude_left = true, wall_th=wall_th, edges=0, l=l0+2, extra_h=0);
                }

              }
              zip_l = 15+wall_th+Zip_h;
              for(k=[0,1]){
                translate([-(zip_l-Zip_h),
                        k*(-l0+wall_th*sqrt(3)+2*zip_fr_edg)-zip_fr_edg-2,
                    wall_th+Min_beam_width]){
                  translate([zip_l-Zip_h,0,0])
                    rotate([0,90,0])
                    translate([0,-Zip_w/2,0])
                    cube([zip_l, Zip_w, Zip_th]);
                  rotate([0,0,0])
                    translate([0,-Zip_w/2,0])
                      cube([zip_l, Zip_w, Zip_h]);
                }
                translate([-Zip_h-wall_th-Min_beam_width,
                        k*(-l0+wall_th*sqrt(3)+2*zip_fr_edg)-zip_fr_edg-2,
                    -1])
                  translate([0,-Zip_w/2,0])
                    cube([Zip_h, Zip_w, zip_l]);
              }

            }
        translate([0,l1/sqrt(3)-2*rad_b-1.3,-1]) // 1.3 chosen arbitrarily
          cylinder(d=2.5, h=wall_th+2, $fn=10);
        fillet_r = 2.5;
        translate([0,(wall_th-fillet_r)*2,0]){
          rotate([0,0,-90-45])
            translate([-fillet_r, -fillet_r,wall_th])
              translate([fillet_r*(1-cos(15)),fillet_r*(1-cos(15)),0])
                inner_round_corner(fillet_r,30,120,2, $fn=4*8);
        }
      } // end diff
      difference(){
        translate([-channel_r2/2,0,0])
          rounded_cube2([channel_r2, channel_l-1, Min_beam_width+wall_th], 1, $fn=20);
        translate([-(channel_r2+2)/2,channel_l,wall_th])
          rotate([90-atan(Min_beam_width/channel_l),0,0])
            cube([channel_r2+2,16,sqrt(channel_l*channel_l + Min_beam_width*Min_beam_width)]);
      }
    } // end union
    translate([0,channel_l,wall_th])
      rotate([90-atan((Min_beam_width-1.3)/(channel_l)),0,0])
        translate([0,1.3,0])
        cylinder(r=1.3, h=sqrt(channel_l*channel_l + Min_beam_width*Min_beam_width)+2, $fn=10);
  } // end diff
}
