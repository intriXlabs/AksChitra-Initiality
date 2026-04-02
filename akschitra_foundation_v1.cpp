// -1 error code for validation origin error
// -2 for validation color error
// -2 for dimensions error
// 1 for rendering issues
// 0 is for succesful run
// aksf means akschitra_foundation

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>

enum aksf_errors{
    POSITION_ERROR=-1, 
    COLOR_ERROR=-2, 
    RENDER_ERROR=1, 
    SUCCESS=0,
    DIMENSION_ERROR=-3
};
enum aksf_color_mode{
    RGB, 
    RAW_RGB, 
    HEX 
};
enum aksf_shape_mode{
    FILL,
    HOLLOW
};

struct aksf_dimension{
    int height=1;
    int width=1;
    int side=1;
    int radius=1;
};

struct aksf_position{
    int row=1;
    int col=1;
};

struct aksf_color{
    aksf_color_mode mode=RGB;
    std::string hexcode="#ff00ff";
    std::string rgb="rgb(255, 0, 255)";
    int red=255;
    int green=0;
    int blue=255;
};

struct aksf_raw_color{
    int red=255;
    int green=0;
    int blue=255;
};

struct aksf_origin{
    aksf_position pos;
    aksf_dimension dmn;
    aksf_color clr;
    aksf_shape_mode sm=FILL;
    // raw_mode means no safety checks - no color or any position or dimension checks
    bool raw_mode=false;
};

class akschitra_foundation{
public:
    int pixel(aksf_origin& origin){
        std::string skeleton=make_pixel(origin);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
    int line(aksf_origin& origin){
        std::string skeleton=make_line(origin);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
    int square(aksf_origin& origin){
        std::string skeleton=make_square(origin);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_dimension(origin))
                return DIMENSION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
    int circle(aksf_origin& origin){
        std::string skeleton=make_circle(origin);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_dimension(origin))
                return DIMENSION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
    int oval(aksf_origin& origin){
        std::string skeleton=make_oval(origin);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_dimension(origin))
                return DIMENSION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
    int triangle(aksf_origin& origin, const std::string& horizontal, const std::string& vertical){
        std::string skeleton=make_triangle(origin, horizontal, vertical);
        aksf_raw_color rc;
        if(origin.raw_mode==false){
            if(!check_position(origin))
                return POSITION_ERROR;
            if(!check_dimension(origin))
                return DIMENSION_ERROR;
            if(!check_colors(origin.clr, rc))
                return COLOR_ERROR;
            std::string shape=apply_color(rc.red, rc.green, rc.blue,skeleton);
            render(shape);
            return SUCCESS;
        }
        // if not - it is raw mode code
        normalize_color(origin.clr, rc);
        std::string shape=apply_color(rc.red, rc.green, rc.blue, skeleton);
        render(shape);
        return SUCCESS;
    }
private:
// checks
    bool check_position(const aksf_origin& o){
        winsize w;
        ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
        if(o.pos.row<1||o.pos.col<1)
            return false;
        if(o.pos.row+o.dmn.height-1>w.ws_row)
            return false;
        if(o.pos.col+o.dmn.width-1>w.ws_col)
            return false;
        return true;
    }
    bool check_dimension(const aksf_origin& o){
        if(o.dmn.height < 1) return false;
        if(o.dmn.width < 1) return false;
        if(o.dmn.side < 1) return false;
        if(o.dmn.radius < 1) return false;
        return true;
    }
    bool check_colors(const aksf_color& c,aksf_raw_color& rc){
        if(c.mode==RGB){
            int r,g,b;
            rgb_stoi(c.rgb,&r,&g,&b);
            if(is_rgb(r,g,b)){
                rc.red=r;
                rc.green=g;
                rc.blue=b;
                return true;
            }
            return false;
        }else if(c.mode==HEX){
            if(ishexcolor(c.hexcode)){
                hex_stoi(c.hexcode,&rc.red,&rc.green,&rc.blue);
                return true;
            }
        }else if(c.mode==RAW_RGB){
            if(!is_rgb(c.red, c.green, c.blue))
                return false;
            rc.red=c.red;
            rc.green=c.green;
            rc.blue=c.blue;
            return true;
        }
        return false;
    }

// abstracted checks
    bool is_rgb(int r,int g,int b){
        if(r>=0&&r<256)
            if(g>=0&&g<256)
                if(b>=0&&b<256)
                    return true;
        return false;
    }
    bool ishexcolor(const std::string& s){
        if(s.size()!=4&&s.size()!=7)
            return false;
        if(s[0]!='#')
            return false;
        for(size_t i=1;i<s.size();i++){
            char c=s[i];
            if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F')))
                return false;
        }
        return true;
    }

// conversions
    void rgb_stoi(const std::string& s, int* r, int* g, int* b)
    {
        sscanf(s.c_str(), "rgb(%d,%d,%d)", r, g, b);
    }
    void hex_stoi(const std::string& s,int* r,int* g,int* b){
        unsigned int rr,gg,bb;
        sscanf(s.c_str(),"#%02x%02x%02x",&rr,&gg,&bb);
        *r=rr;
        *g=gg;
        *b=bb;
    }

// special functions
    void normalize_color(const aksf_color& c, aksf_raw_color& rc) {
        if(c.mode == RGB)
            rgb_stoi(c.rgb, &rc.red, &rc.green, &rc.blue);
        else if(c.mode == HEX)
            hex_stoi(c.hexcode, &rc.red, &rc.green, &rc.blue);
        else
            rc = {c.red, c.green, c.blue};
    }

// skeleton - spapces gets allocated with locations here for shapes
    std::string make_pixel(const aksf_origin& origin){
        std::string skeleton;
        skeleton="\033["+std::to_string(origin.pos.row)+';'+std::to_string(origin.pos.col)+"H ";
        return skeleton;
    }
    std::string make_line(const aksf_origin& origin){
        std::string skeleton;
        int x1=origin.pos.col;
        int y1=origin.pos.row;
        int x2=origin.pos.col+origin.dmn.width-1;
        int y2=origin.pos.row+origin.dmn.height-1;
        int dx=abs(x2-x1);
        int dy=-abs(y2-y1);
        int sx=(x1<x2)?1:-1;
        int sy=(y1<y2)?1:-1;
        int err=dx+dy;
        while(true){
            skeleton+="\033["+std::to_string(y1)+";"+std::to_string(x1)+"H ";
            if(x1==x2 && y1==y2) break;
            int e2=2*err;
            if(e2>=dy){ err+=dy; x1+=sx; }
            if(e2<=dx){ err+=dx; y1+=sy; }
        }
        return skeleton;
    }
    std::string make_square(const aksf_origin& origin) {
        std::string skeleton;
        int start_row = origin.pos.row;
        int start_col = origin.pos.col;
        int side      = origin.dmn.side;
        if(origin.sm == FILL) {
            for(int i = 0; i < side; ++i) {
                skeleton += "\033[" +std::to_string(start_row + i) +";" +std::to_string(start_col) +"H";
                skeleton += std::string(side, ' ');
            }
        } else {
            skeleton += "\033[" +std::to_string(start_row) +";" +std::to_string(start_col) +"H";
            skeleton += std::string(side, ' ');
            for(int i = 1; i < side - 1; ++i) {
                skeleton += "\033[" +std::to_string(start_row + i) +";" +std::to_string(start_col) +"H ";
                if(side > 1) {
                    skeleton += "\033[" +std::to_string(start_row + i) +";" +std::to_string(start_col + side - 1) +"H ";
                }
            }
            if(side > 1) {
                skeleton += "\033[" +std::to_string(start_row + side - 1) +";" +std::to_string(start_col) +"H";
                skeleton += std::string(side, ' ');
            }
        }
        return skeleton;
    }
    std::string make_circle(const aksf_origin& origin){
        std::string skeleton;
        int cx=origin.pos.col;
        int cy=origin.pos.row;
        int r=origin.dmn.radius;
        int r2=r*r;
        int inner=(r-1)*(r-1);
        for(int y=-r;y<=r;++y){
            for(int x=-r;x<=r;++x){
                int d=x*x+y*y;
                bool draw=false;
                if(origin.sm==FILL){
                    if(d<=r2) draw=true;
                }else{
                    if(d<=r2 && d>=inner) draw=true;
                }
                if(draw){
                    skeleton+="\033["+std::to_string(cy+y)+";"+std::to_string(cx+x)+"H ";
                }
            }
        }
        return skeleton;
    }
    std::string make_oval(const aksf_origin& origin){
        std::string skeleton;
        int cx=origin.pos.col;
        int cy=origin.pos.row;
        int w=origin.dmn.width;
        int h=origin.dmn.height;
        int rx=w/2;
        int ry=h/2;
        double rx2=rx*rx;
        double ry2=ry*ry;
        double inner_rx2=(rx-1)*(rx-1);
        double inner_ry2=(ry-1)*(ry-1);
        for(int y=-ry;y<=ry;++y){
            for(int x=-rx;x<=rx;++x){
                double eq=((x*x)/rx2)+((y*y)/ry2);
                bool draw=false;
                if(origin.sm==FILL){
                    if(eq<=1.0) draw=true;
                }else{
                    double inner=((x*x)/inner_rx2)+((y*y)/inner_ry2);
                    if(eq<=1.0 && inner>=1.0) draw=true;
                }
                if(draw){
                    skeleton+="\033["+std::to_string(cy+y)+";"+std::to_string(cx+x)+"H ";
                }
            }
        }
        return skeleton;
    }
    std::string make_triangle(const aksf_origin& origin, const std::string& horizontal, const std::string& vertical){
        std::string skeleton;
        int start_row=origin.pos.row;
        int start_col=origin.pos.col;
        int h=origin.dmn.height;
        int w=origin.dmn.width;
        for(int i=0;i<h;++i){
            int progress=(vertical=="bottom-up")?(i+1):(h-i);
            int col_limit=progress*w/h;
            int draw_col=start_col;
            if(horizontal=="right-left")
                draw_col=start_col+w-col_limit;
            if(origin.sm==FILL){
                skeleton+="\033["+std::to_string(start_row+i)+";"+std::to_string(draw_col)+"H";
                skeleton+=std::string(col_limit,' ');
            }else{
                skeleton+="\033["+std::to_string(start_row+i)+";"+std::to_string(draw_col)+"H ";
                if(col_limit>1)
                    skeleton+="\033["+std::to_string(start_row+i)+";"+std::to_string(draw_col+col_limit-1)+"H ";
            }
        }
        return skeleton;
    }
// body and work - color applies to existing shape here
    std::string apply_color(int r, int g, int b,const std::string& skeleton){
        std::string body;
        body="\033[48;2;"+std::to_string(r)+';'+std::to_string(g)+';'+std::to_string(b)+"m"+skeleton+"\033[0m";
        return body;
    }
// rendering function
    bool render(std::string buffer){
        write(STDOUT_FILENO,buffer.c_str(),buffer.size());
        return true;
    }
};