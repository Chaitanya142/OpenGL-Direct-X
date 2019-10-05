package com.example.ogl_checkerboard;

import android.content.Context;
import android.view.Gravity;
import android.graphics.Color;

import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import android.opengl.GLES32;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;

//import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener {
	private static Context context;
	private GestureDetector gestureDetector;

	private int shaderProgramObject;
	// Shader Objects
	private int gVertexShaderObject;
	private int gFragementShaderObject;

	private int[] vao = new int[1];
	private int[] vbo_position = new int[1];
	private int[] vbo_texCoord = new int[1];

	private int mvpUniform;
	private int samplerUniform;

	private float[] perspectiveProjectionMatrix = new float[16];

	private int[] texture_Check = new int[1];
	
	private final int CheckImageWidth = 64;
	private final int CheckImageHeight = 64;

	private byte[] CheckImage = new byte[CheckImageWidth * CheckImageHeight * 4];

	public GLESView(Context drawingContext) {
		super(drawingContext);

		context = drawingContext;
		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		gestureDetector = new GestureDetector(drawingContext, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int eventaction = event.getAction();
		if (!gestureDetector.onTouchEvent(event)) {
			super.onTouchEvent(event);
		}
		return (true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent e) {
		return (true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e) {
		return (true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e) {
		return (true);
	}

	@Override
	public boolean onDown(MotionEvent e) {
		return (true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		return (true);
	}

	@Override
	public void onLongPress(MotionEvent e) {

	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		UnInitialize();
		System.exit(0);
		return (true);
	}

	@Override
	public void onShowPress(MotionEvent e) {

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		return (true);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		String Version = gl.glGetString(GL10.GL_VERSION);
		String shaderVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);

		System.out.println(Version);
		System.out.println(shaderVersion);

		Initialize();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		Resize(width, height);
	}

	@Override
	public void onDrawFrame(GL10 unused) {
		Display();
	}

	private void Resize(int width, int height) {
		if (height == 0)
			height = 1;

		GLES32.glViewport(0, 0, width, height);

		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);

	}

	private ByteBuffer byteBuffer;
	private FloatBuffer positionBuffer;
	private FloatBuffer texcoordBuffer;

	private void Display() {

		System.out.println("RTR "+texture_Check[0]);

		final float[] rectangleVertices = new float[] {
			-2.0f,-1.0f, 0.0f ,
			-2.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 0.0f };

		final float[] rectangleTexCoords = new float[] { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		GLES32.glUseProgram(shaderProgramObject);

		// Declaration of Matrices
		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];
		float[] translationMatrix = new float[16];

		// Initialize matrices
		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);
		Matrix.setIdentityM(translationMatrix, 0);

		// Transformation

		Matrix.translateM(translationMatrix, 0, +0.0f, 0.0f, -3.6f);

		// Matrix Multiplication

		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, modelViewMatrix, 0);

		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);

		// Send necessary matrices to shader in resp. Uniforms

		GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);

		GLES32.glUniform1i(samplerUniform, 0);		

		// Bind with vao

		GLES32.glBindVertexArray(vao[0]);

		//Position
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position[0]);

		byteBuffer = ByteBuffer.allocateDirect(rectangleVertices.length * 4);

		byteBuffer.order(ByteOrder.nativeOrder());

		positionBuffer = byteBuffer.asFloatBuffer();

		positionBuffer.put(rectangleVertices);

		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 3 * 4, positionBuffer,
				GLES32.GL_DYNAMIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		//Position


		//Texture
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texCoord[0]);

		byteBuffer = ByteBuffer.allocateDirect(rectangleTexCoords.length * 4);

		byteBuffer.order(ByteOrder.nativeOrder());

		texcoordBuffer = byteBuffer.asFloatBuffer();

		texcoordBuffer.put(rectangleTexCoords);

		texcoordBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 2 * 4, texcoordBuffer,
				GLES32.GL_DYNAMIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		//Texture

		// Bind with textures if any

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_Check[0]);

		// Draw

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		// Unbind vao

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);


		rectangleVertices[0] = 1.0f;
		rectangleVertices[1] = -1.0f;
		rectangleVertices[2] = 0.0f;
		rectangleVertices[3] = 1.0f;
		rectangleVertices[4] = 1.0f;
		rectangleVertices[5] = 0.0f;
		rectangleVertices[6] = 2.41421f;
		rectangleVertices[7] = 1.0f;
		rectangleVertices[8] = -1.41421f;
		rectangleVertices[9] = 2.41421f;
		rectangleVertices[10] = -1.0f;
		rectangleVertices[11] = -1.41421f;

		//Position
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position[0]);

		byteBuffer = ByteBuffer.allocateDirect(rectangleVertices.length * 4);

		byteBuffer.order(ByteOrder.nativeOrder());

		positionBuffer = byteBuffer.asFloatBuffer();

		positionBuffer.put(rectangleVertices);

		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 3 * 4, positionBuffer,
				GLES32.GL_DYNAMIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		//Position

		//Texture
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texCoord[0]);

		byteBuffer = ByteBuffer.allocateDirect(rectangleTexCoords.length * 4);

		byteBuffer.order(ByteOrder.nativeOrder());

		texcoordBuffer = byteBuffer.asFloatBuffer();

		texcoordBuffer.put(rectangleTexCoords);

		texcoordBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 2 * 4, texcoordBuffer,
				GLES32.GL_DYNAMIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		//Texture

		// Bind with textures if any

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_Check[0]);

		// Draw

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		// Unbind vao

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		GLES32.glBindVertexArray(0);


		
		
		GLES32.glUseProgram(0);

		requestRender();
	}

	private void Initialize() {

		// Vertex Shader
		// Define Shader Object
		gVertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		// Vertex Shader Code
		final String vertextShaderSourceCode = String.format("#version 320 es " + "\n " + "in vec4 vPosition;"
				+ "in vec2 vTexCoord;" + "uniform mat4 u_mvp_matrix;" + "out vec2 out_TexCoord;" + "void main(void)"
				+ "{" + "gl_Position=u_mvp_matrix * vPosition;" + "out_TexCoord=vTexCoord;" + "} ");

		// Shader object and source code mapping
		GLES32.glShaderSource(gVertexShaderObject, vertextShaderSourceCode);

		// Compile Shader
		GLES32.glCompileShader(gVertexShaderObject);

		// Error Checking
		int[] iShaderCompileStatus = new int[1];
		int[] iInfoLogLength = new int[1];
		String szInfoLog = null;

		GLES32.glGetShaderiv(gVertexShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);

		if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

			GLES32.glGetShaderiv(gVertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

			if (iInfoLogLength[0] > 0) {

				szInfoLog = GLES32.glGetShaderInfoLog(gVertexShaderObject);

				System.out.println("RTR : gVertexShaderObject " + szInfoLog);
				UnInitialize();
				System.exit(0);
			}
		}

		// Fragement Shader
		// Define Shader Object
		gFragementShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		// Vertex Shader Code
		final String fragementShaderSourceCode = String.format("#version 320 es" + "\n " + "precision highp float;"
				+ "uniform highp sampler2D u_sampler;" + "in vec2 out_TexCoord;" + "out vec4 fragColor;"
				+ "void main(void)" + "{" + "fragColor=texture(u_sampler,out_TexCoord);" + "} ");

		// Shader object and source code mapping
		GLES32.glShaderSource(gFragementShaderObject, fragementShaderSourceCode);

		// Compile Shader
		GLES32.glCompileShader(gFragementShaderObject);

		// Error Checking
		iShaderCompileStatus[0] = 0;
		iInfoLogLength[0] = 0;
		szInfoLog = null;

		GLES32.glGetShaderiv(gFragementShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);

		if (iShaderCompileStatus[0] == GLES32.GL_FALSE) {

			GLES32.glGetShaderiv(gFragementShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

			if (iInfoLogLength[0] > 0) {

				szInfoLog = GLES32.glGetShaderInfoLog(gFragementShaderObject);

				System.out.println("RTR : gFragementShaderObject" + szInfoLog);
				UnInitialize();
				System.exit(0);
			}
		}

		// Program Object
		shaderProgramObject = GLES32.glCreateProgram();
		// Attach Shaders
		GLES32.glAttachShader(shaderProgramObject, gVertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, gFragementShaderObject);

		// Prelinking
		// Binding to attribute
		GLES32.glBindAttribLocation(shaderProgramObject, GLESMicros.AMC_ATTRIBUTE_POSITION, "vPosition");

		GLES32.glBindAttribLocation(shaderProgramObject, GLESMicros.AMC_ATTRIBUTE_TEXCOORD0, "vTexCoord");

		// Link Shader Program
		GLES32.glLinkProgram(shaderProgramObject);

		// Error Checking
		int[] iShaderLinkStatus = new int[1];
		iInfoLogLength[0] = 0;
		szInfoLog = null;

		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, iShaderLinkStatus, 0);

		if (iShaderLinkStatus[0] == GLES32.GL_FALSE) {
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

			if (iInfoLogLength[0] > 0) {
				szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);

				System.out.println("RTR : shaderProgramObject" + szInfoLog);
				UnInitialize();
				System.exit(0);

			}
		}

		// Post Linking
		// Retriving Uniform Location
		mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");

		samplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_sampler");

		// Vertices
		// final float[] rectangleVertices = new float[] { 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		// 		-1.0f, 0.0f };

		//final float[] rectangleTexCoords = new float[] { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

		// Create vao
		// Save everying in single set
		GLES32.glGenVertexArrays(1, vao, 0);

		GLES32.glBindVertexArray(vao[0]);

		// Generate Buffer
		GLES32.glGenBuffers(1, vbo_position, 0);
		// Bind Generated Buffer
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position[0]);

		// byteBuffer = ByteBuffer.allocateDirect(rectangleVertices.length * 4);

		// byteBuffer.order(ByteOrder.nativeOrder());

		// positionBuffer = byteBuffer.asFloatBuffer();

		// positionBuffer.put(rectangleVertices);

		// positionBuffer.position(0);

		// Fill Buffer
		// GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, rectangleVertices.length * 4, positionBuffer,
		// 		GLES32.GL_STATIC_DRAW);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4* 3 * 4, null,
				GLES32.GL_DYNAMIC_DRAW);
				
		// Set Vertex Attrib Pointer
		GLES32.glVertexAttribPointer(GLESMicros.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);

		// Enable Vertex Attrib Array
		GLES32.glEnableVertexAttribArray(GLESMicros.AMC_ATTRIBUTE_POSITION);
		// Unbind Buffer
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// Generate Buffer
		GLES32.glGenBuffers(1, vbo_texCoord, 0);
		// Bind Generated Buffer
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texCoord[0]);

		// byteBuffer = ByteBuffer.allocateDirect(rectangleTexCoords.length * 4);

		// byteBuffer.order(ByteOrder.nativeOrder());

		// texcoordBuffer = byteBuffer.asFloatBuffer();

		// texcoordBuffer.put(rectangleTexCoords);

		// texcoordBuffer.position(0);

		// Fill Buffer
		// GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, rectangleTexCoords.length * 4, texcoordBuffer,
		// 		GLES32.GL_STATIC_DRAW);
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 2 * 4, null,
				GLES32.GL_DYNAMIC_DRAW);
		// Set Vertex Attrib Pointer
		GLES32.glVertexAttribPointer(GLESMicros.AMC_ATTRIBUTE_TEXCOORD0, 2, GLES32.GL_FLOAT, false, 0, 0);

		// Enable Vertex Attrib Array
		GLES32.glEnableVertexAttribArray(GLESMicros.AMC_ATTRIBUTE_TEXCOORD0);
		// Unbind Buffer
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// Unbind array
		GLES32.glBindVertexArray(0);

		texture_Check[0] = LoadTexture();

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);
		GLES32.glClearDepthf(1.0f);

		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		GLES32.glEnable(GLES32.GL_TEXTURE_2D);

		Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

	}

	void UnInitialize() {
		if (vbo_position[0] != 0) {
			GLES32.glDeleteBuffers(1, vbo_position, 0);
			vbo_position[0] = 0;
		}
		if (vbo_texCoord[0] != 0) {
			GLES32.glDeleteBuffers(1, vbo_texCoord, 0);
			vbo_texCoord[0] = 0;
		}
		if (vao[0] != 0) {
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
		if (texture_Check[0] != 0) {
			GLES32.glDeleteTextures(1, texture_Check, 0);
			texture_Check[0] = 0;
		}
		if (shaderProgramObject != 0) {
			int[] shaderCount = new int[1];

			GLES32.glUseProgram(shaderProgramObject);

			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, shaderCount, 0);

			if (shaderCount[0] != 0) {

				int shaderNumber;
				int[] shaders = new int[shaderCount[0]];

				GLES32.glGetAttachedShaders(shaderProgramObject, shaderCount[0], shaderCount, 0, shaders, 0);

				for (shaderNumber = 0; shaderNumber < shaderCount[0]; shaderNumber++) {
					GLES32.glDetachShader(shaderProgramObject, shaders[shaderNumber]);
					GLES32.glDeleteShader(shaders[shaderNumber]);
					shaders[shaderNumber] = 0;
				}
			}
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;
			GLES32.glUseProgram(0);
		}
	}


	private int LoadTexture() 
	{	
		int[] texture = new int[1];
		
		MakeCheckImage();
		
		ByteBuffer bytebuffer = ByteBuffer.allocateDirect(CheckImageWidth * CheckImageHeight * 4);
		bytebuffer.order(ByteOrder.nativeOrder());
		bytebuffer.put(CheckImage);
		bytebuffer.position(0);
		
		Bitmap bitmap = Bitmap.createBitmap(CheckImageWidth, CheckImageHeight, Bitmap.Config.ARGB_8888);
		bitmap.copyPixelsFromBuffer(bytebuffer);
		GLES32.glGenTextures(1, texture, 0);
	
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
		
		GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
		
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_REPEAT);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_REPEAT);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_NEAREST);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_NEAREST);
		
		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);
		return texture[0];
	}

	private void MakeCheckImage()
	{
		int i, j, c;
		for(i = 0; i < CheckImageHeight; i++)
		{
			for(j = 0; j < CheckImageWidth; j++)
			{
				c = ((i & 8) ^ (j & 8)) * 255;
				CheckImage[(i * 64 + j) * 4 + 0] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 1] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 2] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 3] = (byte)0;
			}
		}
	}

}

