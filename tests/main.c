#include <stdio.h>
#include <ovg.h>

#define AXIS_PADDING 7
#define AXIS_MAJOR_TICK 100
#define AXIS_MINOR_SPACING 2


void axes(void);

int main(int argc, char **argv) {

    ovg_init(); //initialize OpenGL contexts, etc

    //base fill
    ovg_fill(255,200,128,255);

    //draw some basic shapes
    ovg_rect(10,10,50,100);
    ovg_round_rect(70,10,50,60,15);
    ovg_fill(120,120,255,255);
    ovg_circle(150,30,20);
    ovg_draw();

    //break up into new draw() command,
    //add in some text
    ovg_ellipse(220,30,80,27);
    ovg_line(10,200,300,100);
    ovg_text(50,120,"Demo Shapes",16);
    ovg_draw();

    //test line width
    ovg_stroke_width(1.0);
    ovg_line(20,200,100,200);
    ovg_stroke_width(2.0);
    ovg_line(20,215,100,215);
    ovg_stroke_width(3.0);
    ovg_line(20,230,100,230);
    ovg_stroke_width(4.0);
    ovg_line(20,245,100,245);
    ovg_stroke_width(5.0);
    ovg_line(20,260,100,260);

    //stroke terminations
    ovg_stroke_width(10);
    ovg_stroke_cap(CAP_ROUND);
    ovg_line(120,200,200,200);
    ovg_stroke_cap(CAP_BUTT);
    ovg_line(120,220,200,220);
    ovg_stroke_cap(CAP_SQUARE);
    ovg_line(120,240,200,240);


    //stroke join styles
    int px[3] = {220,230,240},
        py[3] = {200,210,200};
    ovg_stroke_join(JOIN_MITER);
    ovg_polyline(px,py,3);
    py[0] += 25; py[1] +=25; py[2] += 25;
    ovg_stroke_join(JOIN_ROUND);
    ovg_polyline(px,py,3);
    py[0] += 25; py[1] +=25; py[2] += 25;
    ovg_stroke_join(JOIN_BEVEL);
    ovg_polyline(px,py,3);


    //reset strokes
    ovg_stroke_width(1);
    ovg_stroke_cap(CAP_BUTT);


    //bezier curve tests. Arch and squiggle
    ovg_bezier_quad(260,200,280,250,300,200);
    ovg_bezier_cube(260,260,275,300,285,220,300,260);
    ovg_draw();


    //gradients!
    float grad_pts[2] = {0,1};
    unsigned char colors[8] = {
        255,100,100,255,
        20 ,80 ,255,255
    };
    //linear
    ovg_translate(20,300);
    ovg_gradient_linear(2,GRADIENT_PAD,0,0,200,200,grad_pts,colors);
    ovg_rect(0,0,100,100);
    ovg_translate(-20,-300);

    ovg_translate(140,300);
    ovg_gradient_linear(2,GRADIENT_PAD,50,50,100,100,grad_pts,colors);
    ovg_rect(0,0,100,100);
    ovg_translate(-140,-300);

    //normal radial
    ovg_translate(260,300);
    ovg_gradient_radial(2,GRADIENT_PAD,50,50,50,50,50,grad_pts,colors);
    ovg_rect(0,0,100,100);
    ovg_translate(-260,-300);
    ovg_draw();

    //off-center focal point
    ovg_translate(380,300);
    ovg_gradient_radial(2,GRADIENT_PAD,50,50,50,7,50,grad_pts,colors);
    ovg_rect(0,0,100,100);
    ovg_translate(-380,-300);
    ovg_draw();

    ovg_fill(128,128,255,255);


    //very crappy star to test poly* 
    int pt_x[10] = {370,350,320,345,340,370,400,395,420,390},
    	pt_y[10] = {110,75, 75, 50, 10, 35, 10, 50, 75, 75};

    ovg_polygon(pt_x,pt_y,10);

    int i;
    for (i=0; i<10; i++){
    	pt_x[i] += 120;
    }
    ovg_polyline(pt_x,pt_y,10);

    //more basic shapes
    ovg_arc(610,10,100,140,180,-180);



    //test clearing a section of the arc
    ovg_clear_rect(600,0,200,50);


    //transformation tests
    //should be a small diamond above the other shapes
    ovg_translate(500,200);
    ovg_rotate(45);
    ovg_scale(0.5,0.5);
    ovg_rect(-50,-50,100,100);

    //undo transforms in reverse order
    ovg_scale(2.0,2.0);
    ovg_rotate(-45);
    ovg_translate(-500,-200);


    //check placement accuracy
    axes();

    getchar(); //quick way to pause and wait for user input. press enter to close

    //test clearing the whole screen
    ovg_clear();
    ovg_draw();

    //wait to view results of clear, then exit
    getchar(); 
    ovg_cleanup();

	return 0;
}

void axes(void){
    int win_x,win_y,win_w,win_h,
        i;
    ovg_wininfo(&win_x,&win_y,&win_w,&win_h); //get window size
    printf("window is at %d,%d and is %dx%d\n", win_x,win_y,win_w,win_h);

    ovg_line(AXIS_PADDING,AXIS_PADDING,win_w-AXIS_PADDING,AXIS_PADDING);
    ovg_line(AXIS_PADDING,AXIS_PADDING,AXIS_PADDING,win_h-AXIS_PADDING);

    for (i=0; i<win_w; i+=AXIS_MAJOR_TICK/AXIS_MINOR_SPACING){
        if (i % AXIS_MAJOR_TICK == 0){
            ovg_line(i,AXIS_PADDING,i,1);
        } else {
            ovg_line(i,AXIS_PADDING,i,4);
        }  
    }

    for (i=0; i<win_h; i+=AXIS_MAJOR_TICK/AXIS_MINOR_SPACING) {
        if (i % AXIS_MAJOR_TICK == 0){
            ovg_line(AXIS_PADDING,i,1,i);
        } else {
            ovg_line(AXIS_PADDING,i,4,i);
        }
    }

    ovg_draw();



}
