#include <stdio.h>
#include <ovg.h>

#define AXIS_PADDING 7
#define AXIS_MAJOR_TICK 100
#define AXIS_MINOR_SPACING 2


void axes(void);
void basic_shapes(void);
void lines_styles(void);
void fills(void);
void trans(void);
void gradients(void);
void polys(void);
void text(void);
void clear(void);

//convenience testing functions
#define TEST_CASE_SIZE 100
#define TEST_CASE_PADDING 20
#define TEST(FN) \
    do { \
        startcase(); \
        FN \
        endcase(); \
    } while (0)
int test_case_x=TEST_CASE_PADDING,
    test_case_y=TEST_CASE_PADDING;
unsigned char save_colors[8];
void startcase(void);
void endcase(void);
void save_fills(void);
void restore_fills(void);
void reset_styles(void);

//convenience functions
#define _(X) (ovg_free(X))
#define _S(X) (ovg_free(ovg_draw_path(X,STROKE_PATH)))
#define _F(X) (ovg_free(ovg_draw_path(X,FILL_PATH)))
#define _FS(X) (ovg_free(ovg_draw_path(X,STROKE_PATH | FILL_PATH)))
#define _SF _FS

int main(int argc, char **argv) {

    ovg_init(); //initialize OpenGL contexts, etc

    unsigned int w,h;

    ovg_dispinfo(&w,&h);
    printf("Display size is %dx%d\n", w,h);


    ovg_open(0,0,1366,768);

    Path p = ovg_rect(500,500,100,30);
    ovg_mask(p,MASK_SET);
    //ovg_free(p);
    ovg_fill(255,0,0,255);
    ovg_free(ovg_draw_path(ovg_rect(400,490,250,250),FILL_PATH));
    ovg_draw();
    ovg_mask_off();

    basic_shapes();
    fills();
    polys();
    text();
    trans();
    lines_styles();
    gradients();

    //check placement accuracy
    axes();
    clear();

    //wait to view results of clear, then exit
    getchar(); 
    ovg_cleanup();

	return 0;
}

void save_fills(void) {
    ovg_fill_current(&save_colors[0],&save_colors[1],&save_colors[2],&save_colors[3]);
    ovg_stroke_current(&save_colors[4],&save_colors[5],&save_colors[6],&save_colors[7]);
}

void restore_fills(void) {
    ovg_fill(save_colors[0],save_colors[1],save_colors[2],save_colors[3]);
    ovg_stroke(save_colors[4],save_colors[5],save_colors[6],save_colors[7]);
}

void reset_styles(void) {
    ovg_fill(0,0,0,0);
    ovg_stroke(0,0,0,255);
    ovg_stroke_width(1);
    ovg_stroke_cap(CAP_BUTT);
    ovg_stroke_join(JOIN_MITER);
    ovg_dash_phase(0);
    ovg_dash(NULL,0);
}

void startcase(void) {
    save_fills();
    reset_styles();
    ovg_translate(test_case_x,test_case_y);
    _S(ovg_rect(0,0,TEST_CASE_SIZE,TEST_CASE_SIZE));
    restore_fills();
}

void endcase(void) {
    ovg_reset();
    test_case_x += TEST_CASE_SIZE + TEST_CASE_PADDING;

    int win_x,win_y,win_w,win_h;
    ovg_wininfo(&win_x,&win_y,&win_w,&win_h);


    if (test_case_x  + TEST_CASE_SIZE > win_w){
        test_case_x = TEST_CASE_PADDING;
        test_case_y += TEST_CASE_PADDING + TEST_CASE_SIZE;
    }
}

void basic_shapes(void) {
    //draw some basic shapes
    reset_styles();
    ovg_fill(255,200,128,255); //basic fill color
    TEST(
        _F(ovg_rect(5,5,30,30));
        _FS(ovg_rect(50,5,47,30));
        _F(ovg_rect(5,40,90,50));
    );
    TEST(_FS(ovg_round_rect(5,5,50,60,15)););
    TEST(_FS(ovg_circle(50,50,20)););
    TEST(_FS(ovg_ellipse(50,50,80,27)););
    TEST(_S(ovg_line(10,90,85,10)););
    TEST(_FS(ovg_arc(50,10,90,90,0,180)););
}

void fills(void) {
    int sketch_star_x[6] = {5,25,45,0,50,5},
        sketch_star_y[6] = {0,35,0 ,25,25,0};

    ovg_fill(122,200,174,255);
    TEST(
        ovg_fill_rule(FILL_RULE_ALL);
        _FS(ovg_polygon(sketch_star_x,sketch_star_y,5));

        ovg_translate(0,50);
        ovg_fill_rule(FILL_RULE_ALTERNATE);
        _FS(ovg_polygon(sketch_star_x,sketch_star_y,5));
    );
    ovg_draw();
}

void lines_styles(void) {

    TEST(
        //test line width
        ovg_stroke_width(1.0);
        _S(ovg_line(2,10,98,10));
        ovg_stroke_width(2.0);
        _S(ovg_line(2,18,98,18));
        ovg_stroke_width(3.0);
        _S(ovg_line(2,28,98,28));
        ovg_stroke_width(4.0);
        _S(ovg_line(2,38,98,38));
        ovg_stroke_width(5.0);
        _S(ovg_line(2,50,98,50));
        ovg_stroke_width(6);
        _S(ovg_line(2,62,98,62));
        ovg_stroke_width(7);
        _S(ovg_line(2,76,98,76));
        ovg_stroke_width(8);
        _S(ovg_line(2,90,98,90));
    );

    TEST(
        //stroke terminations
        ovg_stroke_width(10);
        ovg_stroke_cap(CAP_ROUND);
        _S(ovg_line(10,20,90,20));
        ovg_stroke_cap(CAP_BUTT);
        _S(ovg_line(10,50,90,50));
        ovg_stroke_cap(CAP_SQUARE);
        _S(ovg_line(10,80,90,80));
    );

    //stroke join styles
    int px[3] = {40,50,60},
        py[3] = {10,30,10};
    TEST(
        ovg_stroke_width(10);
        ovg_stroke_join(JOIN_MITER);
        _S(ovg_polyline(px,py,3));
        py[0] += 30; py[1] +=30; py[2] += 30;
        ovg_stroke_join(JOIN_ROUND);
        _S(ovg_polyline(px,py,3));
        py[0] += 30; py[1] +=30; py[2] += 30;
        ovg_stroke_join(JOIN_BEVEL);
        _S(ovg_polyline(px,py,3));
    );

    TEST(
        //bezier curve tests. Arch and squiggle
        _S(ovg_bezier_quad(10,10,45,60,80,10));
        _S(ovg_bezier_cube(10,60,25,100,35,20,50,60));
    );

    //dashed lines
    int dash[4] = {5,10,15,20};
    TEST(
        ovg_stroke_width(3);
        //normal dash, repeated twice
        ovg_dash(dash,4);
        _S(ovg_line(10,10,90,10));
        //above that, offset the position
        ovg_dash_phase(10);
        _S(ovg_line(10,25,90,25));

        //reset phase
        ovg_dash_phase(0);
        _S(ovg_line(10,40,90,40));
    );

    ovg_dash(NULL,0); //this is how to turn off dashing
    ovg_draw();
}

void trans(void) {
    //transformation tests
    //should be a small diamond above the other shapes
    Path diamond;
    float x,y,w,h;

    ovg_fill(50,128,255,255);
    TEST(
        ovg_translate(40,40);
        ovg_rotate(45);
        ovg_scale(0.5,0.5);
        diamond = ovg_draw_path(ovg_rect(-50,-50,100,100),FILL_PATH | STROKE_PATH);
        //bounds checking of transformed box:
        ovg_bounds(diamond, &x,&y,&w,&h);
        ovg_free(diamond);
    );
    //draw bounds
    ovg_stroke(255,0,0,255);
    _S(ovg_rect((int)x,(int)y,(int)w,(int)h));
    ovg_draw();

    ovg_stroke(0,0,0,255); //undo red stroke

    //test path interpolation

    int dec_x[10] = { 50,21, 2, 2,21,50,79,98,98,79},
        dec_y[10] = {100,89,65,34,10, 0,10,34,65,89};
    int star_x[10] = {50,30,0,25,20,50,80,75,100,70},
        star_y[10] = {100,65, 65, 40, 0, 25, 0, 40, 65, 65};
    Path star, dec;
    star = ovg_polygon(star_x,star_y,10);
    dec = ovg_polygon(dec_x,dec_y,10);

    int i;
    for (i=0; i<6; i++){
        TEST(
            _FS(ovg_interpolate(star,dec,i*0.2));
        );
    }
    ovg_free(star);
    ovg_free(dec);
}

void gradients(void) {
    //gradients!
    float grad_pts[2] = {0,1};
    unsigned char colors[8] = {
        255,100,100,255,
        20 ,80 ,255,255
    };
    //linear
    TEST(
        ovg_gradient_linear(2,GRADIENT_PAD,0,0,200,200,grad_pts,colors);
        _F(ovg_rect(0,0,100,100));
    );
    //linear, tighter
    TEST(
        ovg_gradient_linear(2,GRADIENT_PAD,50,50,100,100,grad_pts,colors);
        _F(ovg_rect(0,0,100,100));
    );
    //normal radial
    TEST(
        ovg_gradient_radial(2,GRADIENT_PAD,50,50,50,50,50,grad_pts,colors);
        _F(ovg_rect(0,0,100,100));
    );
    //off-center focal point
    TEST(
        ovg_gradient_radial(2,GRADIENT_PAD,50,50,50,7,50,grad_pts,colors);
        _F(ovg_rect(0,0,100,100));
    );


    //test gradient repeat types
    float complex_grad_pts[4] = {0,0.33,0.66,1};
    unsigned char complex_colors[16] = {
        255,0,0,255,
        0,255,0,255,
        0,0,255,255,
        255,255,255,255
    };
    //linear, pad
    TEST(
        ovg_gradient_linear(4,GRADIENT_PAD,20,20,60,60,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );
    //linear, repeat
    TEST(
        ovg_gradient_linear(4,GRADIENT_REPEAT,20,20,60,60,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );
    //linear, reflect
    TEST(
        ovg_gradient_linear(4,GRADIENT_REFLECT,20,20,60,60,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );
    //radial, pad
    TEST(
        ovg_gradient_radial(4,GRADIENT_PAD,50,50,50,50,50,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );
    //radial, repeat
    TEST(
        ovg_gradient_radial(4,GRADIENT_REPEAT,50,50,50,50,50,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );
    //radial, reflect
    TEST(
        ovg_gradient_radial(4,GRADIENT_REFLECT,50,50,50,50,50,complex_grad_pts,complex_colors);
        _F(ovg_rect(0,0,100,100));
    );


    ovg_draw();
}
void polys(void) {
    //very crappy star to test poly* 
    int pt_x[10] = {50, 30, 0,25,20,50,80,75,100,70},
        pt_y[10] = {100,65,65,40, 0,25, 0,40, 65,65};

    ovg_fill(255,200,128,255); //basic fill color
    TEST(_FS(ovg_polygon(pt_x,pt_y,10)););
    TEST(_S(ovg_polyline(pt_x,pt_y,10)););
}


void text(void) {
    ovg_fill(0,0,0,255);
    TEST(
        ovg_text(4,70,"Demo",24);
        ovg_text(4,40,"Shapes",16);
    );
    ovg_draw();
}

void clear(void) {
    ovg_fill(220,80,80,255);
    TEST(
        _FS(ovg_rect(10,10,80,80));
        ovg_clear_rect(70,10,20,20);
    );
    ovg_draw();


    getchar();

    ovg_clear();
    ovg_draw();
}

void axes(void){
    int win_x,win_y,win_w,win_h,
        i;
    ovg_wininfo(&win_x,&win_y,&win_w,&win_h); //get window size
    printf("window is at %d,%d and is %dx%d\n", win_x,win_y,win_w,win_h);

    _S(ovg_line(AXIS_PADDING,AXIS_PADDING,win_w-AXIS_PADDING,AXIS_PADDING));
    _S(ovg_line(AXIS_PADDING,AXIS_PADDING,AXIS_PADDING,win_h-AXIS_PADDING));

    for (i=0; i<win_w; i+=AXIS_MAJOR_TICK/AXIS_MINOR_SPACING){
        if (i % AXIS_MAJOR_TICK == 0){
            _S(ovg_line(i,AXIS_PADDING,i,1));
        } else {
            _S(ovg_line(i,AXIS_PADDING,i,4));
        }  
    }

    for (i=0; i<win_h; i+=AXIS_MAJOR_TICK/AXIS_MINOR_SPACING) {
        if (i % AXIS_MAJOR_TICK == 0){
            _S(ovg_line(AXIS_PADDING,i,1,i));
        } else {
            _S(ovg_line(AXIS_PADDING,i,4,i));
        }
    }

    ovg_draw();



}
