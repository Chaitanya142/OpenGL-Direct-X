#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>
#include<string>
#include<string.h>

#include</usr/include/GL/glew.h>
#include</usr/include/GL/gl.h>
#include</usr/include/GL/glx.h>
#include</usr/include/SOIL/SOIL.h>
#include"vmath.h"
#include<math.h>
#include"Sphere.h"

using namespace std;
using namespace vmath;

bool bFullScreen=false;
Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColorMap;
Window gWindow;
int giWindowWidth=800;
int giWindowHeight=600;
GLXContext gGLXContext;
bool bDone=false;
FILE *gpFile ; 

typedef GLXContext(* glXCreateContextAttribsARBProc)(
Display *,
GLXFBConfig,
GLXContext,
bool,
GLint *        
);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB=NULL;
GLXFBConfig gGLXFBConfig;

GLint gShaderProgramObject;

enum {
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD0
};

GLuint vao_quad;
GLuint vao_triangle;
GLuint vao_point;

GLuint vbo_position_quad;
GLuint vbo_position_triangle;
GLuint vbo_position_point;

GLuint vbo_color_quad;
GLuint vbo_color_triangle;
GLuint vbo_color_point;

//GLuint vbo_texCoord_rectangle;

GLuint mvpUniform;
//GLuint samplerUniform;

mat4 perspectiveProjectionMatrix;

GLfloat D_R_OrangeColor = 1.0f;
GLfloat D_G_OrangeColor = 0.6f;
GLfloat D_B_OrangeColor = 0.2f;
GLfloat D_R_GreenColor = 0.074f;
GLfloat D_G_GreenColor = 0.533f;
GLfloat D_B_GreenColor = 0.2f;

GLfloat Line_Orange_R = 1.0f;
GLfloat Line_Orange_G = 0.6f;
GLfloat Line_Orange_B = 0.2f;
GLfloat Line_White_R = 1.00f;
GLfloat Line_White_G = 1.00f;
GLfloat Line_White_B = 1.00f;
GLfloat Line_Green_R = 0.074f;
GLfloat Line_Green_G = 0.533f;
GLfloat Line_Green_B = 0.2f;

GLint stage = 1;
GLfloat I_1_XPosition = -2.25f;
GLfloat A_XPosition = 2.25f;
GLfloat N_YPosition = 2.25f;
GLfloat I_2_YPosition = -2.25f;
GLfloat Plane1_Y_Position = 0.00f;
GLfloat Plane1_Z_Rotation_Angle = -90.0f;

GLfloat Plane1_X_Position = -2.5f;
GLfloat angle = 0.0f;
GLfloat Line_X_Position = -1.28f;

Sphere planeFrontSphere;

void Update(void);

int main(void)
{
void CreateWindow(void);
void ToggleFullScreen(void);
void UnInitialize();
void Initialize(void);
void Resize(int,int);
void DisplayOpenGL(void);
gpFile = fopen("ChaitanyaLog.c", "w+");
int winWidth=giWindowWidth;
int winHeight=giWindowHeight;
char keys[26];
CreateWindow();
XEvent event;
KeySym keysym;
Initialize();

while(bDone==false){
    while(XPending(gpDisplay)){
        XNextEvent(gpDisplay,&event);
        switch(event.type)
        {
                case MapNotify:
                break;

                //to get keysym code details type xev in terminal and press enter . press key to get details
                case KeyPress:
                        keysym=XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
                        // static char buffer [60];
                        // static std::string s = std::to_string(keysym);
                        // static const char *cstr = s.c_str();
                        // fputs(cstr, filePointer) ;                   
                        switch(keysym)
                        {
                        case XK_Escape:
                        bDone=true;
                        break;
                        default:
                        break;
                        }
                        XLookupString(&event.xkey,keys,sizeof(keys),NULL,NULL);
                        switch(keys[0])
                        {
                        case 'F':
                        case 'f':       
                                if(bFullScreen==false)
                                {
                                        ToggleFullScreen();
                                        bFullScreen=true;
                                }
                                else
                                {
                                        ToggleFullScreen();
                                        bFullScreen=false;
                                }
                        break;
                        }
                break;
                case ButtonPress:
                        switch(event.xbutton.button)
                        {
                        case 1:
                        break;
                        case 2:
                        break;
                        case 3:
                        break;
                        default:
                        break;
                        }
                break;
                case MotionNotify:
                break;
                case ConfigureNotify:
                        winWidth=event.xconfigure.width;
                        winHeight=event.xconfigure.height;
                        Resize(winWidth,winHeight);
                break;
                case Expose:
                break;
                case DestroyNotify:
                break;
                case 33:
                    bDone=true;
                default:
                break;
        }        
    }

    Update();
    DisplayOpenGL();
}

//fclose(filePointer); 
UnInitialize();
return(0);
}

void CreateWindow(void)
{
void UnInitialize();
XSetWindowAttributes winAttribs;
int defaultScreen;
int defaultDepth;
int styleMask;

GLXFBConfig *pGLXFBConfig=NULL;
GLXFBConfig bestGLXFBConfig;
XVisualInfo *pTempXVisualInfo=NULL;
int iNoOfFBConfigs=0;

int bestFrameBufferConfig=-1;
int bestNoOfSamples=-1;
int worstFrameBufferConfig=-1;
int worstNoOfSamples=999;

static int FrameBufferAttributes[]={
GLX_X_RENDERABLE,true,
GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
GLX_RENDER_TYPE,GLX_RGBA_BIT,
GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
GLX_RED_SIZE,8,
GLX_GREEN_SIZE,8,
GLX_BLUE_SIZE,8,
GLX_ALPHA_SIZE,8,
GLX_DEPTH_SIZE,24,
GLX_STENCIL_SIZE,8,
GLX_DOUBLEBUFFER,true,
None    
};

gpDisplay=XOpenDisplay(NULL);
if(gpDisplay == NULL){
        printf("ERROR : Unable To Open X Display.\nExitting Now...\n");
        UnInitialize();
        exit(1);
}
defaultScreen=XDefaultScreen(gpDisplay);

//defaultDepth=DefaultDepth(gpDisplay,defaultScreen);  

pGLXFBConfig=glXChooseFBConfig(gpDisplay,
defaultScreen,
FrameBufferAttributes,
&iNoOfFBConfigs);

for(int i=0;i<iNoOfFBConfigs;i++){
        pTempXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,
        pGLXFBConfig[i]);

        if(pTempXVisualInfo){
                int sampleBuffers,samples;
                
                glXGetFBConfigAttrib(gpDisplay,
                pGLXFBConfig[i],
                GLX_SAMPLE_BUFFERS,
                &sampleBuffers);

                glXGetFBConfigAttrib(gpDisplay,
                pGLXFBConfig[i],
                GLX_SAMPLES,
                &samples);

                if(bestFrameBufferConfig<0 || sampleBuffers && samples>bestNoOfSamples){
                        bestFrameBufferConfig=i;
                        bestNoOfSamples=samples;
                }
                if(worstFrameBufferConfig<0 || !sampleBuffers && samples<worstNoOfSamples){
                        worstFrameBufferConfig=i;
                        worstNoOfSamples=samples;
                }

        }
        XFree(pTempXVisualInfo);
}

bestGLXFBConfig=pGLXFBConfig[bestFrameBufferConfig];
gGLXFBConfig=bestGLXFBConfig;
XFree(pGLXFBConfig);

gpXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,
bestGLXFBConfig);

if(gpXVisualInfo==NULL){
        printf("ERROR : Unable To Get A Visual.\nExitting Now...\n");
        UnInitialize();
        exit(1);
}

winAttribs.border_pixel=0;
winAttribs.background_pixmap=0;
winAttribs.colormap=XCreateColormap(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),gpXVisualInfo->visual,AllocNone);
gColorMap=winAttribs.colormap;
winAttribs.background_pixel=BlackPixel(gpDisplay,defaultScreen);
winAttribs.border_pixmap=0;
winAttribs.event_mask=ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
styleMask= CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
gWindow=XCreateWindow(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),0,0,giWindowWidth,giWindowHeight,0,gpXVisualInfo->depth,InputOutput,gpXVisualInfo->visual,styleMask,&winAttribs);
if(!gWindow)
{
        printf("ERROR : Failed To Create Main Window.\nExitting Now...\n");
        UnInitialize();
        exit(1);
}
XStoreName(gpDisplay,gWindow,"OGL Window");
Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
XMapWindow(gpDisplay,gWindow);
}

void ToggleFullScreen(void){

Atom wm_state;
Atom fullscreen;
XEvent xev={0};
wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False); 
memset(&xev,0,sizeof(xev)); 
xev.type=ClientMessage;
xev.xclient.window=gWindow;
xev.xclient.message_type=wm_state;
xev.xclient.format=32;
xev.xclient.data.l[0]=bFullScreen?0:1;
fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);
xev.xclient.data.l[1]=fullscreen;
XSendEvent(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),False,StructureNotifyMask,&xev);

}

void UnInitialize()
{
    if(bFullScreen){
        ToggleFullScreen();
    }
	if (vbo_color_quad) {
		glDeleteBuffers(1, &vbo_color_quad);
		vbo_color_quad = 0;
	}

	if (vbo_position_quad) {
		glDeleteBuffers(1, &vbo_position_quad);
		vbo_position_quad = 0;
	}
	if (vao_quad) {
		glDeleteVertexArrays(1, &vao_quad);
		vao_quad = 0;
	}
	if (vao_point) {
		glDeleteVertexArrays(1, &vao_point);
		vao_point = 0;
	}
	if (vbo_position_point) {
		glDeleteBuffers(1, &vbo_position_point);
		vbo_position_point = 0;
	}

	if (vbo_color_point) {
		glDeleteBuffers(1, &vbo_color_point);
		vbo_color_point = 0;
	}

	if (vbo_position_triangle) {
		glDeleteBuffers(1, &vbo_position_triangle);
		vbo_position_triangle = 0;
	}

	if (vbo_color_triangle) {
		glDeleteBuffers(1, &vbo_color_triangle);
		vbo_color_triangle = 0;
	}

	if (vao_triangle) {
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
	}

	if (gShaderProgramObject) {
		GLsizei shaderCount;
		GLsizei shaderNumber;
		glUseProgram(gShaderProgramObject);

		glGetProgramiv(gShaderProgramObject,
			GL_ATTACHED_SHADERS,
			&shaderCount);

		GLuint * pShaders = (GLuint *)malloc(shaderCount * sizeof(GLuint));

		if (pShaders) {
			glGetAttachedShaders(gShaderProgramObject,
				shaderCount,
				&shaderCount,
				pShaders);

			for (shaderNumber = 0; shaderNumber < shaderCount; shaderNumber++) {
				glDetachShader(gShaderProgramObject, pShaders[shaderNumber]);
				glDeleteShader(pShaders[shaderNumber]);
				pShaders[shaderNumber] = 0;
			}
			free(pShaders);
		}
		glDeleteProgram(gShaderProgramObject);
		gShaderProgramObject = 0;
		glUseProgram(0);
	}

GLXContext currentGLXContext=glXGetCurrentContext();
if(currentGLXContext != NULL && currentGLXContext== gGLXContext){
    glXMakeCurrent(gpDisplay,0,0);

    if(gGLXContext){
        glXDestroyContext(gpDisplay,gGLXContext);
    }
}
if(gWindow)
{
        XDestroyWindow(gpDisplay,gWindow);
}
if(gColorMap)
{
        XFreeColormap(gpDisplay,gColorMap);
}
if(gpXVisualInfo)
{
        free(gpXVisualInfo);
        gpXVisualInfo=NULL;
}
if(gpDisplay){
        XCloseDisplay(gpDisplay);
        gpDisplay=NULL;
}
if(gGLXContext){
        XFree(gGLXContext);
        gGLXContext=NULL;
}

}

void Initialize(void){
    //Function Declaration
        void Resize(int, int);

        glXCreateContextAttribsARB=(glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

        if(glXCreateContextAttribsARB==NULL){
                printf("ERROR : Failed To Get glXCreateContextAttribsARB .\nExitting Now...\n");
                UnInitialize();
                exit(1);
        }

        GLint Attribs[]={
            GLX_CONTEXT_MAJOR_VERSION_ARB,4,
            GLX_CONTEXT_MINOR_VERSION_ARB,5,
            GLX_CONTEXT_PROFILE_MASK_ARB,GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        };    

        gGLXContext=glXCreateContextAttribsARB(gpDisplay,
                                        gGLXFBConfig,
                                        0,
                                        true,
                                        Attribs);

        if(!gGLXContext){
                GLint Attribs[]={
                                GLX_CONTEXT_MAJOR_VERSION_ARB,1,
                                GLX_CONTEXT_MINOR_VERSION_ARB,0,
                                None
                };    

                gGLXContext=glXCreateContextAttribsARB(gpDisplay,
                                                gGLXFBConfig,
                                                0,
                                                true,
                                                Attribs);
        }

        if(!glXIsDirect(gpDisplay,gGLXContext)){
                printf("ERROR : The Obtained context is not H/W rendering context...\n");
        }else{
                printf("The Obtained context is H/W rendering context...\n");
        }
         
        glXMakeCurrent(gpDisplay,
        gWindow,
        gGLXContext);

    GLenum result;
    result=glewInit();
    
    if(result!=GLEW_OK){
                printf("ERROR : Failed To Initialize GLEW.\nExitting Now...\n");
                UnInitialize();
                exit(1);
    }

	
	//Shader Objects
	GLint gVertexShaderObject;
	GLint gFragementShaderObject;

	//Vertex Shader
	//Define Shader Object
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//Vertex Shader Code
	const GLchar* vertextShaderSourceCode =
		"#version 430 core " \
		"\n " \
		"in vec4 vPosition;" \
		"in vec4 vColor;" \
		"uniform mat4 u_mvp_matrix;" \
		"out vec4 out_color;" \
		"void main(void)" \
		"{" \
		"gl_Position=u_mvp_matrix * vPosition;" \
		"out_color=vColor;"\
		"} ";

	//"in vec4 vColor;" \
	//"out vec4 out_color;" \
		//"out_color=vColor;"\

	//"in vec2 vTexCoord;" \
	//"out vec2 out_TexCoord;" \
	//"out_TexCoord=vTexCoord;"\


	//Shader object and source code mapping
	glShaderSource(gVertexShaderObject, 1, &vertextShaderSourceCode, NULL);

	//Compile Shader
	glCompileShader(gVertexShaderObject);

	//Error Checking
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar * szInfoLog = NULL;

	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar *)malloc(iInfoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject,
					iInfoLogLength,
					&written,
					szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log %s\n", szInfoLog);
				free(szInfoLog);
				UnInitialize();
				exit(0);
			}
		}
	}

	//Fragement Shader
	//Define Shader Object
	gFragementShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//Fragement Shader Code
	const GLchar* fragementShaderSourceCode =
		"#version 430 core " \
		"\n " \
		"in vec4 out_color;"\
		"out vec4 fragColor;"
		"void main(void)" \
		"{" \
		"fragColor=out_color;" \
		"} ";


	//"in vec4 out_color;"\
	//"fragColor=out_color;" \

	//"uniform sampler2D u_sampler;"
	//"in vec2 out_TexCoord;"
	//"fragColor=texture(u_sampler,out_TexCoord);" \


	//Shader object and source code mapping
	glShaderSource(gFragementShaderObject, 1, &fragementShaderSourceCode, NULL);

	//Compile Shader
	glCompileShader(gFragementShaderObject);

	//Error Checking
	iShaderCompileStatus = 0;
	iInfoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(gFragementShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(gFragementShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar *)malloc(iInfoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetShaderInfoLog(gFragementShaderObject,
					iInfoLogLength,
					&written,
					szInfoLog);
				fprintf(gpFile, "Fragement Shader Compilation Log %s\n", szInfoLog);
				free(szInfoLog);
				UnInitialize();
				exit(0);
			}
		}
	}

	//Program Object
	gShaderProgramObject = glCreateProgram();
	//Attach Shaders
	glAttachShader(gShaderProgramObject, gVertexShaderObject);
	glAttachShader(gShaderProgramObject, gFragementShaderObject);

	//Prelinking
	//Binding to attribute
	glBindAttribLocation(gShaderProgramObject,
		AMC_ATTRIBUTE_POSITION,
		"vPosition");

	//glBindAttribLocation(gShaderProgramObject,
	//	AMC_ATTRIBUTE_TEXCOORD0,
	//	"vTexCoord");

	glBindAttribLocation(gShaderProgramObject,
		AMC_ATTRIBUTE_COLOR,
		"vColor");

	//Link Shader Program
	glLinkProgram(gShaderProgramObject);

	//Error Checking
	GLint iShaderLinkStatus = 0;

	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderLinkStatus);

	if (iShaderLinkStatus == GL_FALSE) {
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar *)malloc(iInfoLogLength);
			if (szInfoLog != NULL) {
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject,
					iInfoLogLength,
					&written,
					szInfoLog);
				fprintf(gpFile, "Program Link Log %s\n", szInfoLog);
				free(szInfoLog);
				UnInitialize();
				exit(0);
			}
		}
	}

	//Post Linking
	//Retriving Uniform Location
	mvpUniform = glGetUniformLocation(gShaderProgramObject,
		"u_mvp_matrix");

	/*samplerUniform = glGetUniformLocation(gShaderProgramObject,
		"u_sampler");*/


		//RECTANGLE
	glGenVertexArrays(1, &vao_quad);

	glBindVertexArray(vao_quad);

	//Generate Buffer
	glGenBuffers(1, &vbo_position_quad);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_position_quad);
	//Fill Buffer
	//glBufferData(GL_ARRAY_BUFFER,
	//	sizeof(rectangleVertices),
	//	rectangleVertices,
	//	GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,
		4 * 3 * sizeof(float),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//COLOR Generate Buffer
	glGenBuffers(1, &vbo_color_quad);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_color_quad);
	//Fill Buffer
	//glBufferData(GL_ARRAY_BUFFER,
	//	sizeof(rectangleColors),
	//	rectangleColors,
	//	GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,
		4 * 3 * sizeof(float),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//Save everying in single set
	glGenVertexArrays(1, &vao_point);

	glBindVertexArray(vao_point);

	//POINT
	//Generate Buffer
	glGenBuffers(1, &vbo_position_point);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_position_point);
	//Fill Buffer
	glBufferData(GL_ARRAY_BUFFER,
		3 * sizeof(float)*((2 * 3.1415) / 0.001),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_color_point);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_color_point);
	//Fill Buffer
	glBufferData(GL_ARRAY_BUFFER,
		3 * sizeof(float)*((2 * 3.1415) / 0.001),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind array
	glBindVertexArray(0);



	//Create vao
	//Save everying in single set
	glGenVertexArrays(1, &vao_triangle);

	glBindVertexArray(vao_triangle);


	//TRIANGLE
	//Generate Buffer
	glGenBuffers(1, &vbo_position_triangle);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_position_triangle);
	//Fill Buffer
	glBufferData(GL_ARRAY_BUFFER,
		3 * 3 * sizeof(float),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_color_triangle);
	//Bind Generated Buffer
	glBindBuffer(GL_ARRAY_BUFFER,
		vbo_color_triangle);
	//Fill Buffer
	glBufferData(GL_ARRAY_BUFFER,
		3 * 3 * sizeof(float),
		NULL,
		GL_DYNAMIC_DRAW);
	//Set Vertex Attrib Pointer
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	//Enable Vertex Attrib Array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
	//Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind array
	glBindVertexArray(0);

	planeFrontSphere.setCenter(0.21f,0.0f);
	planeFrontSphere.setTextureColor(0.729f , 0.886f , 0.933f);//(1.0f*255.0f, 1.0f*255.0f, 1.0f * 255); 
	planeFrontSphere.set(0.07f, 30, 30);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);

	perspectiveProjectionMatrix = mat4::identity();
	
    Resize(giWindowWidth,giWindowHeight);

}

//Function Resize
void Resize(int width, int height) {
   if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = perspective(45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);

}

//Function Display
void DisplayOpenGL(void) {
	void DrawLetter(char letter);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject);

	//Declaration of Matrices
	mat4 modelViewMatrix;
	mat4 modelViewProjectionMatrix;
	mat4 translationMatrix;
	mat4 rotationMatrix;


	if (stage > 0) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation

		translationMatrix = translate(I_1_XPosition, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('I');

		//Unbind vao_quad

	}

	if (stage > 2) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation

		translationMatrix = translate(-1.0f, N_YPosition, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('N');

		//Unbind vao_quad

	}

	if (stage > 4) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation

		translationMatrix = translate(-0.25f, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('D');

		//Unbind vao_quad

	}

	if (stage > 3) {

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation

		translationMatrix = translate(0.25f, I_2_YPosition, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('I');

		//Unbind vao_quad

	}
	if (stage > 1) {

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation

		translationMatrix = translate(A_XPosition, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('A');
		//DrawLetter('T');

		//Unbind vao_quad

	}
	if (stage == 8) {
		Plane1_Y_Position = ((Plane1_X_Position - 1.3)*(Plane1_X_Position - 1.3) / (4 * 0.15)) - 0.0f;

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, Plane1_Y_Position, -3.0f);

		rotationMatrix = rotate(Plane1_Z_Rotation_Angle, 0.0f, 0.0f, 1.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad


		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad


		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, -Plane1_Y_Position, -3.0f);
		rotationMatrix = rotate(-Plane1_Z_Rotation_Angle, 0.0f, 0.0f, 1.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad
	}

	if (stage == 7) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, 0.0f, -3.0f);
		rotationMatrix = rotate(-Plane1_Z_Rotation_Angle, 0.0f, 0.0f, 1.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad

	}

	if ((stage == 7 || stage == 8) && Plane1_X_Position > -1.02f && Plane1_X_Position < 1.40f) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(0.0f, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('L');

		//Unbind vao_quad

	}

	if (stage > 7 && Plane1_X_Position > 1.40f) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(A_XPosition, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('T');

		//Unbind vao_quad
	}

	if (stage > 7 && Plane1_X_Position > 1.40f && Line_X_Position <= 1.10f) {
		Line_X_Position = Line_X_Position + 0.006f;

		if (Line_White_R > 0.0f)
			Line_White_R = Line_White_R - Line_White_R / 256;
		if (Line_White_G > 0.0f)
			Line_White_G = Line_White_G - Line_White_G / 256;
		if (Line_White_B > 0.0f)
			Line_White_B = Line_White_B - Line_White_B / 256;

		if (Line_Orange_R > 0.0f)
			Line_Orange_R = Line_Orange_R - Line_Orange_R / 256;
		if (Line_Orange_G > 0.0f)
			Line_Orange_G = Line_Orange_G - Line_Orange_G / 256;
		if (Line_Orange_B > 0.0f)
			Line_Orange_B = Line_Orange_B - Line_Orange_B / 256;

		if (Line_Green_R > 0.0f)
			Line_Green_R = Line_Green_R - Line_Green_R / 256;
		if (Line_Green_G > 0.0f)
			Line_Green_G = Line_Green_G - Line_Green_G / 256;
		if (Line_Green_B > 0.0f)
			Line_Green_B = Line_Green_B - Line_Green_B / 256;

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(0.0f, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('L');

		//Unbind vao_quad

	}

	if (stage > 7 && Plane1_X_Position > 1.40f) {
		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(A_XPosition, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('T');

		//Unbind vao_quad

	}


	if (stage == 6)
	{
		Plane1_Y_Position = ((Plane1_X_Position + 1.3)*(Plane1_X_Position + 1.3) / (4 * 0.15)) - 0.0f;

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, Plane1_Y_Position, -3.0f);
		rotationMatrix = rotate(Plane1_Z_Rotation_Angle, 0.0f, 0.0f, 1.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, 0.0f, -3.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad

		//Initialize matrices
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();

		//Transformation
		translationMatrix = translate(Plane1_X_Position, -1 * (Plane1_Y_Position), -3.0f);
		rotationMatrix = rotate(-Plane1_Z_Rotation_Angle, 0.0f, 0.0f, 1.0f);

		//Matrix Multiplication
		modelViewMatrix = translationMatrix * rotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		//Send necessary matrices to shader in resp. Uniforms

		glUniformMatrix4fv(mvpUniform,
			1,
			GL_FALSE,
			modelViewProjectionMatrix);

		//Bind with vao_quad

		//Bind with textures if any

		DrawLetter('P');

		//Unbind vao_quad

	}

	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}
void Update(void)
{
	// switch (stage) {
	// case 1:
	// 	//Print I
	// 	if (I_1_XPosition < -1.5f)
	// 		I_1_XPosition = I_1_XPosition + 0.00022f;
	// 	else {
	// 		I_1_XPosition = -1.5f;
	// 		stage++;
	// 	}
	// 	break;
	// case 2:
	// 	//Print A
	// 	if (A_XPosition > 0.75f)
	// 		A_XPosition = A_XPosition - 0.00022f;
	// 	else {
	// 		A_XPosition = 0.75f;
	// 		stage++;
	// 	}
	// 	break;
	// case 3:
	// 	//Print N
	// 	if (N_YPosition > 0.00f)
	// 		N_YPosition = N_YPosition - 0.00022f;
	// 	else {
	// 		N_YPosition = 0.00f;
	// 		stage++;
	// 	}
	// 	break;
	// case 4:
	// 	//Print I
	// 	if (I_2_YPosition < 0.00f)
	// 		I_2_YPosition = I_2_YPosition + 0.00022f;
	// 	else {
	// 		I_2_YPosition = 0.00f;
	// 		stage++;
	// 	}
	// 	break;
	// case 5:
	// 	//Print D
	// 	if (D_R_OrangeColor < 1.0f)
	// 		D_R_OrangeColor = D_R_OrangeColor + 1.0f / 5050;
	// 	if (D_G_OrangeColor < 0.6f)
	// 		D_G_OrangeColor = D_G_OrangeColor + 0.6f / 5050;
	// 	if (D_B_OrangeColor < 0.2f)
	// 		D_B_OrangeColor = D_B_OrangeColor + 0.2f / 5050;

	// 	if (D_R_GreenColor < 0.074f)
	// 		D_R_GreenColor = D_R_GreenColor + 0.074f / 5050;
	// 	if (D_G_GreenColor < 0.533f)
	// 		D_G_GreenColor = D_G_GreenColor + 0.533f / 5050;
	// 	if (D_B_GreenColor < 0.2f)
	// 		D_B_GreenColor = D_B_GreenColor + 0.2f / 5050;

	// 	if (D_R_OrangeColor >= 1.0f && D_G_OrangeColor >= 0.6f && D_B_OrangeColor >= 0.2f && D_R_GreenColor >= 0.074f && D_G_GreenColor >= 0.533f && D_B_GreenColor >= 0.2f)
	// 		stage++;
	// 	break;
	// case 6:
	// 	if (Plane1_X_Position < -1.3f) {
	// 		Plane1_X_Position = Plane1_X_Position + 0.0008f;
	// 		Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.2 / 0.0008);//0.015f
	// 	}
	// 	else
	// 		stage++;
	// 	break;
	// case 7:
	// 	if (Plane1_X_Position < 1.3f)
	// 		Plane1_X_Position = Plane1_X_Position + 0.0008f;
	// 	else
	// 		stage++;
	// 	break;
	// case 8:
	// 	if (Plane1_X_Position < 2.5f)
	// 	{
	// 		Plane1_X_Position = Plane1_X_Position + 0.0008f;
	// 		Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.3 / 0.0008);
	// 	}
	// 	else
	// 		stage++;
	// 	break;
	// }
		switch (stage) {
	case 1:
		//Print I
		if (I_1_XPosition < -1.5f)
			//I_1_XPosition = I_1_XPosition + 0.00022f;
			I_1_XPosition = I_1_XPosition + 0.005f;
		else {
			I_1_XPosition = -1.5f;
			stage++;
		}
		break;
	case 2:
		//Print A
		if (A_XPosition > 0.75f)
			//A_XPosition = A_XPosition - 0.00022f;
			A_XPosition = A_XPosition - 0.005f;
		else {
			A_XPosition = 0.75f;
			stage++;
		}
		break;
	case 3:
		//Print N
		if (N_YPosition > 0.00f)
			//N_YPosition = N_YPosition - 0.00022f;
			N_YPosition = N_YPosition - 0.005f;
		else {
			N_YPosition = 0.00f;
			stage++;
		}
		break;
	case 4:
		//Print I
		if (I_2_YPosition < 0.00f)
			//I_2_YPosition = I_2_YPosition + 0.00022f;
			I_2_YPosition = I_2_YPosition + 0.005f;
		else {
			I_2_YPosition = 0.00f;
			stage++;
		}
		break;
	case 5:
		//Print D
		if (D_R_OrangeColor < 1.0f)
			//D_R_OrangeColor = D_R_OrangeColor + 1.0f / 5050;
			D_R_OrangeColor = D_R_OrangeColor + 1.0f / 1000;
		if (D_G_OrangeColor < 0.6f)
			D_G_OrangeColor = D_G_OrangeColor + 0.6f / 1000;
		if (D_B_OrangeColor < 0.2f)
			D_B_OrangeColor = D_B_OrangeColor + 0.2f / 1000;

		if (D_R_GreenColor < 0.074f)
			D_R_GreenColor = D_R_GreenColor + 0.074f / 1000;
		if (D_G_GreenColor < 0.533f)
			D_G_GreenColor = D_G_GreenColor + 0.533f / 1000;
		if (D_B_GreenColor < 0.2f)
			D_B_GreenColor = D_B_GreenColor + 0.2f / 1000;

		if (D_R_OrangeColor >= 1.0f && D_G_OrangeColor >= 0.6f && D_B_OrangeColor >= 0.2f && D_R_GreenColor >= 0.074f && D_G_GreenColor >= 0.533f && D_B_GreenColor >= 0.2f)
			stage++;
		break;
	case 6:
		if (Plane1_X_Position < -1.3f) {
			//Plane1_X_Position = Plane1_X_Position + 0.0008f;
			Plane1_X_Position = Plane1_X_Position + 0.001f;
			//Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.2 / 0.0008);//0.015f
			Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.2 / 0.001);//0.015f
		}
		else
			stage++;
		break;
	case 7:
		if (Plane1_X_Position < 1.3f)
			//Plane1_X_Position = Plane1_X_Position + 0.0008f;
			Plane1_X_Position = Plane1_X_Position + 0.001f;
		else
			stage++;
		break;
	case 8:
		if (Plane1_X_Position < 2.5f)
		{
			//Plane1_X_Position = Plane1_X_Position + 0.0008f;
			//Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.3 / 0.0008);
            Plane1_X_Position = Plane1_X_Position + 0.001f;
			Plane1_Z_Rotation_Angle = Plane1_Z_Rotation_Angle + 90.0f / (1.3 / 0.001);
		}
		else
			stage++;
		break;
	}

}


GLfloat rectangleVertices[] = {
		0.275f, 1.0f, 0.0f,
		0.225f, 1.0f, 0.0f,
		0.225f, -1.0f, 0.0f,
		0.275f, -1.0f, 0.0f
};

GLfloat rectangleColorCoords[] = {
1.0f, 0.6f, 0.2f,
1.0f, 0.6f, 0.2f,
0.074f, 0.533f, 0.2f,
0.074f, 0.533f, 0.2f
};



void DrawLetter(char letter) {
	switch (letter)
	{
	case 'I':
		glBindVertexArray(vao_quad);

		rectangleVertices[0] = 0.275f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.225f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.225f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.275f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;



		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		glBindVertexArray(0);

		break;

	case 'N':
		glBindVertexArray(vao_quad);

		//Left side of N
		rectangleVertices[0] = 0.05f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.0f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.00f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.05f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;


		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		//Middle part of N
		rectangleVertices[0] = 0.05f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.00f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.45f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.50f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		//Right Side of N
		rectangleVertices[0] = 0.50f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.45f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.45f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.50f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;


		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		glBindVertexArray(0);

		break;

	case 'D':
		glBindVertexArray(vao_quad);

		rectangleVertices[0] = 0.05f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.0f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.00f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.05f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = D_R_OrangeColor;
		rectangleColorCoords[1] = D_G_OrangeColor;
		rectangleColorCoords[2] = D_B_OrangeColor;
		rectangleColorCoords[3] = D_R_OrangeColor;
		rectangleColorCoords[4] = D_G_OrangeColor;
		rectangleColorCoords[5] = D_B_OrangeColor;
		rectangleColorCoords[6] = D_R_GreenColor;
		rectangleColorCoords[7] = D_G_GreenColor;
		rectangleColorCoords[8] = D_B_GreenColor;
		rectangleColorCoords[9] = D_R_GreenColor;
		rectangleColorCoords[10] = D_G_GreenColor;
		rectangleColorCoords[11] = D_B_GreenColor;




		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		rectangleVertices[0] = 0.5f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.01f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.01f;
		rectangleVertices[7] = 0.95f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.5f;
		rectangleVertices[10] = 0.95f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = D_R_OrangeColor;
		rectangleColorCoords[1] = D_G_OrangeColor;
		rectangleColorCoords[2] = D_B_OrangeColor;
		rectangleColorCoords[3] = D_R_OrangeColor;
		rectangleColorCoords[4] = D_G_OrangeColor;
		rectangleColorCoords[5] = D_B_OrangeColor;
		rectangleColorCoords[6] = D_R_OrangeColor;
		rectangleColorCoords[7] = D_G_OrangeColor;
		rectangleColorCoords[8] = D_B_OrangeColor;
		rectangleColorCoords[9] = D_R_OrangeColor;
		rectangleColorCoords[10] = D_G_OrangeColor;
		rectangleColorCoords[11] = D_B_OrangeColor;


		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.50f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.45f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.45f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.50f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = D_R_OrangeColor;
		rectangleColorCoords[1] = D_G_OrangeColor;
		rectangleColorCoords[2] = D_B_OrangeColor;
		rectangleColorCoords[3] = D_R_OrangeColor;
		rectangleColorCoords[4] = D_G_OrangeColor;
		rectangleColorCoords[5] = D_B_OrangeColor;
		rectangleColorCoords[6] = D_R_GreenColor;
		rectangleColorCoords[7] = D_G_GreenColor;
		rectangleColorCoords[8] = D_B_GreenColor;
		rectangleColorCoords[9] = D_R_GreenColor;
		rectangleColorCoords[10] = D_G_GreenColor;
		rectangleColorCoords[11] = D_B_GreenColor;


		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.50f;
		rectangleVertices[1] = -0.95f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.01f;
		rectangleVertices[4] = -0.95f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.01f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.50f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = D_R_GreenColor;
		rectangleColorCoords[1] = D_G_GreenColor;
		rectangleColorCoords[2] = D_B_GreenColor;
		rectangleColorCoords[3] = D_R_GreenColor;
		rectangleColorCoords[4] = D_G_GreenColor;
		rectangleColorCoords[5] = D_B_GreenColor;
		rectangleColorCoords[6] = D_R_GreenColor;
		rectangleColorCoords[7] = D_G_GreenColor;
		rectangleColorCoords[8] = D_B_GreenColor;
		rectangleColorCoords[9] = D_R_GreenColor;
		rectangleColorCoords[10] = D_G_GreenColor;
		rectangleColorCoords[11] = D_B_GreenColor;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		glBindVertexArray(0);

		break;
	case 'A':
		glBindVertexArray(vao_quad);

		rectangleVertices[0] = 0.275f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.225f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.00f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.05f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		rectangleVertices[0] = 0.275f;
		rectangleVertices[1] = 1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.225f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.45f;
		rectangleVertices[7] = -1.0f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.50f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);
		break;

		glBindVertexArray(0);

	case 'T':
		glBindVertexArray(vao_quad);

		//Tricolor of A
		rectangleVertices[0] = 0.380f;
		rectangleVertices[1] = 0.06f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.12f;
		rectangleVertices[4] = 0.06f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.12f;
		rectangleVertices[7] = 0.02f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.380f;
		rectangleVertices[10] = 0.02f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 1.0f;
		rectangleColorCoords[7] = 0.6f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 1.0f;
		rectangleColorCoords[10] = 0.6f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.380f;
		rectangleVertices[1] = 0.02f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.12f;
		rectangleVertices[4] = 0.02f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.12f;
		rectangleVertices[7] = -0.02f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.380f;
		rectangleVertices[10] = -0.02f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 1.0f;
		rectangleColorCoords[2] = 1.0f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 1.0f;
		rectangleColorCoords[5] = 1.0f;
		rectangleColorCoords[6] = 1.0f;
		rectangleColorCoords[7] = 1.0f;
		rectangleColorCoords[8] = 1.0f;
		rectangleColorCoords[9] = 1.0f;
		rectangleColorCoords[10] = 1.0f;
		rectangleColorCoords[11] = 1.0f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.380f;
		rectangleVertices[1] = -0.02f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.12f;
		rectangleVertices[4] = -0.02f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.12f;
		rectangleVertices[7] = -0.06f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.380f;
		rectangleVertices[10] = -0.06f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.074f;
		rectangleColorCoords[1] = 0.533f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.074f;
		rectangleColorCoords[4] = 0.533f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		glBindVertexArray(0);

		break;

	case 'L':
		glBindVertexArray(vao_quad);

		if (Plane1_X_Position < 1.40) {
			//Line Red
			rectangleVertices[0] = Plane1_X_Position - 0.27f;
			rectangleVertices[1] = 0.06f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = 0.06f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = 0.02f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = Plane1_X_Position - 0.27f;
			rectangleVertices[10] = 0.02f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 1.0f;
			rectangleColorCoords[1] = 0.6f;
			rectangleColorCoords[2] = 0.2f;
			rectangleColorCoords[3] = 1.0f;
			rectangleColorCoords[4] = 0.6f;
			rectangleColorCoords[5] = 0.2f;
			rectangleColorCoords[6] = 1.0f;
			rectangleColorCoords[7] = 0.6f;
			rectangleColorCoords[8] = 0.2f;
			rectangleColorCoords[9] = 1.0f;
			rectangleColorCoords[10] = 0.6f;
			rectangleColorCoords[11] = 0.2f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);

			// W
			rectangleVertices[0] = Plane1_X_Position - 0.27f;
			rectangleVertices[1] = 0.02f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = 0.02f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = -0.02f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = Plane1_X_Position - 0.27f;
			rectangleVertices[10] = -0.02f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 1.0f;
			rectangleColorCoords[1] = 1.0f;
			rectangleColorCoords[2] = 1.0f;
			rectangleColorCoords[3] = 1.0f;
			rectangleColorCoords[4] = 1.0f;
			rectangleColorCoords[5] = 1.0f;
			rectangleColorCoords[6] = 1.0f;
			rectangleColorCoords[7] = 1.0f;
			rectangleColorCoords[8] = 1.0f;
			rectangleColorCoords[9] = 1.0f;
			rectangleColorCoords[10] = 1.0f;
			rectangleColorCoords[11] = 1.0f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);


			// G		
			rectangleVertices[0] = Plane1_X_Position - 0.27f;
			rectangleVertices[1] = -0.02f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = -0.02f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = -0.06f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = Plane1_X_Position - 0.27f;
			rectangleVertices[10] = -0.06f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 0.074f;
			rectangleColorCoords[1] = 0.533f;
			rectangleColorCoords[2] = 0.2f;
			rectangleColorCoords[3] = 0.074f;
			rectangleColorCoords[4] = 0.533f;
			rectangleColorCoords[5] = 0.2f;
			rectangleColorCoords[6] = 0.074f;
			rectangleColorCoords[7] = 0.533f;
			rectangleColorCoords[8] = 0.2f;
			rectangleColorCoords[9] = 0.074f;
			rectangleColorCoords[10] = 0.533f;
			rectangleColorCoords[11] = 0.2f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);

		}
		else {

			//R
			rectangleVertices[0] = 1.1f;
			rectangleVertices[1] = 0.06f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = 0.06f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = 0.02f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = 1.1f;
			rectangleVertices[10] = 0.02f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 1.0f;
			rectangleColorCoords[1] = 0.6f;
			rectangleColorCoords[2] = 0.2f;
			rectangleColorCoords[3] = 1.0f;
			rectangleColorCoords[4] = 0.6f;
			rectangleColorCoords[5] = 0.2f;
			rectangleColorCoords[6] = 1.0f;
			rectangleColorCoords[7] = 0.6f;
			rectangleColorCoords[8] = 0.2f;
			rectangleColorCoords[9] = 1.0f;
			rectangleColorCoords[10] = 0.6f;
			rectangleColorCoords[11] = 0.2f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);

			// W
			rectangleVertices[0] = 1.1f;
			rectangleVertices[1] = 0.02f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = 0.02f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = -0.02f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = 1.1f;
			rectangleVertices[10] = -0.02f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 1.0f;
			rectangleColorCoords[1] = 1.0f;
			rectangleColorCoords[2] = 1.0f;
			rectangleColorCoords[3] = 1.0f;
			rectangleColorCoords[4] = 1.0f;
			rectangleColorCoords[5] = 1.0f;
			rectangleColorCoords[6] = 1.0f;
			rectangleColorCoords[7] = 1.0f;
			rectangleColorCoords[8] = 1.0f;
			rectangleColorCoords[9] = 1.0f;
			rectangleColorCoords[10] = 1.0f;
			rectangleColorCoords[11] = 1.0f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);

			// G		
			rectangleVertices[0] = 1.1f;
			rectangleVertices[1] = -0.02f;
			rectangleVertices[2] = 0.0f;
			rectangleVertices[3] = Line_X_Position;
			rectangleVertices[4] = -0.02f;
			rectangleVertices[5] = 0.0f;
			rectangleVertices[6] = Line_X_Position;
			rectangleVertices[7] = -0.06f;
			rectangleVertices[8] = 0.0f;
			rectangleVertices[9] = 1.1f;
			rectangleVertices[10] = -0.06f;
			rectangleVertices[11] = 0.0f;

			rectangleColorCoords[0] = 0.074f;
			rectangleColorCoords[1] = 0.533f;
			rectangleColorCoords[2] = 0.2f;
			rectangleColorCoords[3] = 0.074f;
			rectangleColorCoords[4] = 0.533f;
			rectangleColorCoords[5] = 0.2f;
			rectangleColorCoords[6] = 0.074f;
			rectangleColorCoords[7] = 0.533f;
			rectangleColorCoords[8] = 0.2f;
			rectangleColorCoords[9] = 0.074f;
			rectangleColorCoords[10] = 0.533f;
			rectangleColorCoords[11] = 0.2f;

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_position_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleVertices,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER,
				vbo_color_quad);
			glBufferData(GL_ARRAY_BUFFER,
				4 * 3 * sizeof(float),
				rectangleColorCoords,
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN,
				0,
				4);
		}
		glBindVertexArray(0);

		break;
		//Plane
	case 'P':
		glBindVertexArray(vao_quad);

		//Smoke R
		rectangleVertices[0] = -0.24f;
		rectangleVertices[1] = 0.06f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.40f;
		rectangleVertices[4] = 0.06f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.40;
		rectangleVertices[7] = -0.02f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.24;
		rectangleVertices[10] = -0.02f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.0f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.0f;
		rectangleColorCoords[9] = 1.0f;
		rectangleColorCoords[10] = 0.6f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		//Smoke W

		rectangleVertices[0] = -0.24f;
		rectangleVertices[1] = 0.02f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.40f;
		rectangleVertices[4] = 0.02f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.40;
		rectangleVertices[7] = -0.02f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.24;
		rectangleVertices[10] = -0.02f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 1.0f;
		rectangleColorCoords[2] = 1.0f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.0f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.0f;
		rectangleColorCoords[9] = 1.0f;
		rectangleColorCoords[10] = 1.0f;
		rectangleColorCoords[11] = 1.0f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		//Smoke G

		rectangleVertices[0] = -0.24f;
		rectangleVertices[1] = -0.02f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.40f;
		rectangleVertices[4] = -0.02f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.40;
		rectangleVertices[7] = -0.06f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.24;
		rectangleVertices[10] = -0.06f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.074f;
		rectangleColorCoords[1] = 0.533f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.0f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.0f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);





		rectangleVertices[0] = 0.20f;
		rectangleVertices[1] = 0.07f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.20f;
		rectangleVertices[4] = 0.07f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.20f;
		rectangleVertices[7] = -0.07f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.20;
		rectangleVertices[10] = -0.07f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.729f;
		rectangleColorCoords[1] = 0.886f;
		rectangleColorCoords[2] = 0.933f;
		rectangleColorCoords[3] = 0.729f;
		rectangleColorCoords[4] = 0.886f;
		rectangleColorCoords[5] = 0.933f;
		rectangleColorCoords[6] = 0.729f;
		rectangleColorCoords[7] = 0.886f;
		rectangleColorCoords[8] = 0.933f;
		rectangleColorCoords[9] = 0.729f;
		rectangleColorCoords[10] = 0.886f;
		rectangleColorCoords[11] = 0.933f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		//Back Wings

		rectangleVertices[0] = -0.10f;
		rectangleVertices[1] = 0.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.27f;
		rectangleVertices[4] = 0.12f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.31f;
		rectangleVertices[7] = 0.12f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.24f;
		rectangleVertices[10] = 0.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.729f;
		rectangleColorCoords[1] = 0.886f;
		rectangleColorCoords[2] = 0.933f;
		rectangleColorCoords[3] = 0.729f;
		rectangleColorCoords[4] = 0.886f;
		rectangleColorCoords[5] = 0.933f;
		rectangleColorCoords[6] = 0.729f;
		rectangleColorCoords[7] = 0.886f;
		rectangleColorCoords[8] = 0.933f;
		rectangleColorCoords[9] = 0.729f;
		rectangleColorCoords[10] = 0.886f;
		rectangleColorCoords[11] = 0.933f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = -0.10f;
		rectangleVertices[1] = 0.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.27f;
		rectangleVertices[4] = -0.12f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.31f;
		rectangleVertices[7] = -0.12f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.24f;
		rectangleVertices[10] = 0.0f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.729f;
		rectangleColorCoords[1] = 0.886f;
		rectangleColorCoords[2] = 0.933f;
		rectangleColorCoords[3] = 0.729f;
		rectangleColorCoords[4] = 0.886f;
		rectangleColorCoords[5] = 0.933f;
		rectangleColorCoords[6] = 0.729f;
		rectangleColorCoords[7] = 0.886f;
		rectangleColorCoords[8] = 0.933f;
		rectangleColorCoords[9] = 0.729f;
		rectangleColorCoords[10] = 0.886f;
		rectangleColorCoords[11] = 0.933f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		//Letter I
		rectangleVertices[0] = -0.05f;
		rectangleVertices[1] = 0.05f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.07f;
		rectangleVertices[4] = 0.05f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.07f;
		rectangleVertices[7] = -0.05f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.05f;
		rectangleVertices[10] = -0.05f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 1.0f;
		rectangleColorCoords[1] = 0.6f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 1.0f;
		rectangleColorCoords[4] = 0.6f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		//Letter A			
		rectangleVertices[0] = 0.00f;
		rectangleVertices[1] = 0.05f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.02f;
		rectangleVertices[4] = 0.05f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.04f;
		rectangleVertices[7] = -0.05f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = -0.02f;
		rectangleVertices[10] = -0.05f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.0f;
		rectangleColorCoords[1] = 0.0f;
		rectangleColorCoords[2] = 0.530f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.530f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.530f;
		rectangleColorCoords[9] = 0.0f;
		rectangleColorCoords[10] = 0.0f;
		rectangleColorCoords[11] = 0.530f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.00f;
		rectangleVertices[1] = 0.05f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.02f;
		rectangleVertices[4] = 0.05f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.04f;
		rectangleVertices[7] = -0.05f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.02f;
		rectangleVertices[10] = -0.05f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.0f;
		rectangleColorCoords[1] = 0.0f;
		rectangleColorCoords[2] = 0.530f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.530f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.530f;
		rectangleColorCoords[9] = 0.0f;
		rectangleColorCoords[10] = 0.0f;
		rectangleColorCoords[11] = 0.530f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		rectangleVertices[0] = 0.02f;
		rectangleVertices[1] = 0.01f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = -0.02f;
		rectangleVertices[4] = 0.01f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = -0.02f;
		rectangleVertices[7] = -0.01f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.02f;
		rectangleVertices[10] = -0.01f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.0f;
		rectangleColorCoords[1] = 0.0f;
		rectangleColorCoords[2] = 0.530f;
		rectangleColorCoords[3] = 0.0f;
		rectangleColorCoords[4] = 0.0f;
		rectangleColorCoords[5] = 0.530f;
		rectangleColorCoords[6] = 0.0f;
		rectangleColorCoords[7] = 0.0f;
		rectangleColorCoords[8] = 0.530f;
		rectangleColorCoords[9] = 0.0f;
		rectangleColorCoords[10] = 0.0f;
		rectangleColorCoords[11] = 0.530f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);


		//Letter F

		rectangleVertices[0] = 0.06f;
		rectangleVertices[1] = 0.05f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.04f;
		rectangleVertices[4] = 0.05f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.04f;
		rectangleVertices[7] = -0.05f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.06f;
		rectangleVertices[10] = -0.05f;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.074f;
		rectangleColorCoords[1] = 0.533f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.074f;
		rectangleColorCoords[4] = 0.533f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.09f;
		rectangleVertices[1] = 0.05f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.06f;
		rectangleVertices[4] = 0.05f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.06f;
		rectangleVertices[7] = 0.035f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.09f;
		rectangleVertices[10] = 0.035;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.074f;
		rectangleColorCoords[1] = 0.533f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.074f;
		rectangleColorCoords[4] = 0.533f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		rectangleVertices[0] = 0.09f;
		rectangleVertices[1] = 0.02f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 0.06f;
		rectangleVertices[4] = 0.02f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 0.06f;
		rectangleVertices[7] = 0.005f;
		rectangleVertices[8] = 0.0f;
		rectangleVertices[9] = 0.09f;
		rectangleVertices[10] = 0.005;
		rectangleVertices[11] = 0.0f;

		rectangleColorCoords[0] = 0.074f;
		rectangleColorCoords[1] = 0.533f;
		rectangleColorCoords[2] = 0.2f;
		rectangleColorCoords[3] = 0.074f;
		rectangleColorCoords[4] = 0.533f;
		rectangleColorCoords[5] = 0.2f;
		rectangleColorCoords[6] = 0.074f;
		rectangleColorCoords[7] = 0.533f;
		rectangleColorCoords[8] = 0.2f;
		rectangleColorCoords[9] = 0.074f;
		rectangleColorCoords[10] = 0.533f;
		rectangleColorCoords[11] = 0.2f;

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_quad);
		glBufferData(GL_ARRAY_BUFFER,
			4 * 3 * sizeof(float),
			rectangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLE_FAN,
			0,
			4);

		glBindVertexArray(0);

		GLfloat triangleVertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		};

		GLfloat triangleColorCoords[] = {
			0.729f,
			0.886f,
			0.933f,
			0.729f,
			0.886f,
			0.933f,
			0.729f,
			0.886f,
			0.933f
		};


		glBindVertexArray(vao_triangle);
		//Upper Wing
		triangleVertices[0] = -0.05f;
		triangleVertices[1] = 0.07f;
		triangleVertices[2] = 0.0f;
		triangleVertices[3] = 0.05f;
		triangleVertices[4] = 0.07f;
		triangleVertices[5] = 0.0f;
		triangleVertices[6] = -0.15f;
		triangleVertices[7] = 0.22f;
		triangleVertices[8] = 0.0f;

		/*triangleColorCoords[0] = 0.729f;
		triangleColorCoords[1] = 0.886f;
		triangleColorCoords[2] = 0.933f;
		triangleColorCoords[3] = 1.0f;
		triangleColorCoords[4] = 0.6f;
		triangleColorCoords[5] = 0.2f;
		triangleColorCoords[6] = 0.074f;
		triangleColorCoords[7] = 0.533f;
		triangleColorCoords[8] = 0.2f;*/

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES,
			0,
			3);

		glBindVertexArray(0);


		glBindVertexArray(vao_triangle);
		triangleVertices[0] = -0.05f;
		triangleVertices[1] = -0.07f;
		triangleVertices[2] = 0.0f;
		triangleVertices[3] = 0.05f;
		triangleVertices[4] = -0.07f;
		triangleVertices[5] = 0.0f;
		triangleVertices[6] = -0.15f;
		triangleVertices[7] = -0.22f;
		triangleVertices[8] = 0.0f;

		/*triangleColorCoords[0] = 0.729f;
		triangleColorCoords[1] = 0.886f;
		triangleColorCoords[2] = 0.933f;
		triangleColorCoords[3] = 1.0f;
		triangleColorCoords[4] = 0.6f;
		triangleColorCoords[5] = 0.2f;
		triangleColorCoords[6] = 0.074f;
		triangleColorCoords[7] = 0.533f;
		triangleColorCoords[8] = 0.2f;*/

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES,
			0,
			3);

		glBindVertexArray(0);

		//Lower Wing

		glBindVertexArray(vao_triangle);
		triangleVertices[0] = 0.05f;
		triangleVertices[1] = 0.07f;
		triangleVertices[2] = 0.0f;
		triangleVertices[3] = -0.10f;
		triangleVertices[4] = 0.22f;
		triangleVertices[5] = 0.0f;
		triangleVertices[6] = -0.15f;
		triangleVertices[7] = 0.22f;
		triangleVertices[8] = 0.0f;

		/*triangleColorCoords[0] = 0.729f;
		triangleColorCoords[1] = 0.886f;
		triangleColorCoords[2] = 0.933f;
		triangleColorCoords[3] = 1.0f;
		triangleColorCoords[4] = 0.6f;
		triangleColorCoords[5] = 0.2f;
		triangleColorCoords[6] = 0.074f;
		triangleColorCoords[7] = 0.533f;
		triangleColorCoords[8] = 0.2f;*/

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES,
			0,
			3);

		glBindVertexArray(0);


		glBindVertexArray(vao_triangle);
		triangleVertices[0] = 0.05f;
		triangleVertices[1] = -0.07f;
		triangleVertices[2] = 0.0f;
		triangleVertices[3] = -0.10f;
		triangleVertices[4] = -0.22f;
		triangleVertices[5] = 0.0f;
		triangleVertices[6] = -0.15f;
		triangleVertices[7] = -0.22f;
		triangleVertices[8] = 0.0f;

		/*triangleColorCoords[0] = 0.729f;
		triangleColorCoords[1] = 0.886f;
		triangleColorCoords[2] = 0.933f;
		triangleColorCoords[3] = 1.0f;
		triangleColorCoords[4] = 0.6f;
		triangleColorCoords[5] = 0.2f;
		triangleColorCoords[6] = 0.074f;
		triangleColorCoords[7] = 0.533f;
		triangleColorCoords[8] = 0.2f;*/

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_triangle);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES,
			0,
			3);

		glBindVertexArray(0);



		//glBegin(GL_TRIANGLES);

////Upper Wing
//glVertex3f(-0.05f, 0.07f, 0.0f);
//glVertex3f(0.05f, 0.07f, 0.0f);
//glVertex3f(-0.15f, 0.22f, 0.0f);

//glVertex3f(-0.05f, -0.07f, 0.0f);
//glVertex3f(0.05f, -0.07f, 0.0f);
//glVertex3f(-0.15f, -0.22f, 0.0f);
////Lower Wing
//glVertex3f(0.05f, 0.07f, 0.0f);
//glVertex3f(-0.10f, 0.22f, 0.0f);
//glVertex3f(-0.15f, 0.22f, 0.0f);

//glVertex3f(0.05f, -0.07f, 0.0f);
//glVertex3f(-0.10f, -0.22f, 0.0f);
//glVertex3f(-0.15f, -0.22f, 0.0f);

//glEnd();


////Front Circle

//glBegin(GL_POINTS);

////glColor3f(1.0f, 1.0f, 1.0f);

//for (float radius = 0.0f; radius <= 0.07f; radius = radius + 0.001f) {
//	for (angle = 0.00f; angle < 2 * 3.14159265; angle = angle + 0.01f) {
//		glVertex3f(cos(angle)*radius + 0.21f, sin(angle)*radius, 0.0f);
//	}
//}
//glEnd();

		/*glColor3f(0.729f, 0.886f, 0.933f);

		if (!initCircle) {
			for (float radius = 0.0f; radius <= 0.07f; radius = radius + 0.001f) {
				for (angle = 0.00f; angle < 2 * 3.14159265; angle = angle + 0.01f) {
					glVertex3f(cos(angle)*radius + 0.21f, sin(angle)*radius, 0.0f);
				}
			}
		}

		glBindVertexArray(vao_point);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_position_point);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleVertices,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER,
			vbo_color_point);
		glBufferData(GL_ARRAY_BUFFER,
			3 * 3 * sizeof(float),
			triangleColorCoords,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES,
			0,
			3);

		glBindVertexArray(0);
*/

		planeFrontSphere.drawWithColor();

		break;

	}
}