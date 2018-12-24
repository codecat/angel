namespace GLSL
{
string Syntax = """
#if !defined(GL_ES) && __VERSION__ < 140
	#define lowp
	#define mediump
	#define highp
#endif
#if defined(VERTEX) || __VERSION__ > 100 || defined(GL_FRAGMENT_PRECISION_HIGH)
	#define LOVE_HIGHP_OR_MEDIUMP highp
#else
	#define LOVE_HIGHP_OR_MEDIUMP mediump
#endif
#define number float
#define Image sampler2D
#define ArrayImage sampler2DArray
#define CubeImage samplerCube
#define VolumeImage sampler3D
#if __VERSION__ >= 300 && !defined(LOVE_GLSL1_ON_GLSL3)
	#define DepthImage sampler2DShadow
	#define DepthArrayImage sampler2DArrayShadow
	#define DepthCubeImage samplerCubeShadow
#endif
#define extern uniform
#ifdef GL_EXT_texture_array
#extension GL_EXT_texture_array : enable
#endif
#ifdef GL_OES_texture_3D
#extension GL_OES_texture_3D : enable
#endif
#ifdef GL_OES_standard_derivatives
#extension GL_OES_standard_derivatives : enable
#endif
""";

string Uniforms = """
// According to the GLSL ES 1.0 spec, uniform precision must match between stages,
// but we can't guarantee that highp is always supported in fragment shaders...
// We *really* don't want to use mediump for these in vertex shaders though.
uniform LOVE_HIGHP_OR_MEDIUMP mat4 ViewSpaceFromLocal;
uniform LOVE_HIGHP_OR_MEDIUMP mat4 ClipSpaceFromView;
uniform LOVE_HIGHP_OR_MEDIUMP mat4 ClipSpaceFromLocal;
uniform LOVE_HIGHP_OR_MEDIUMP mat3 ViewNormalFromLocal;
uniform LOVE_HIGHP_OR_MEDIUMP vec4 love_ScreenSize;

// Compatibility
#define TransformMatrix ViewSpaceFromLocal
#define ProjectionMatrix ClipSpaceFromView
#define TransformProjectionMatrix ClipSpaceFromLocal
#define NormalMatrix ViewNormalFromLocal
""";

string Functions = """
#ifdef GL_ES
	#if __VERSION__ >= 300 || defined(GL_EXT_texture_array)
		precision lowp sampler2DArray;
	#endif
	#if __VERSION__ >= 300 || defined(GL_OES_texture_3D)
		precision lowp sampler3D;
	#endif
	#if __VERSION__ >= 300
		precision lowp sampler2DShadow;
		precision lowp samplerCubeShadow;
		precision lowp sampler2DArrayShadow;
	#endif
#endif

#if __VERSION__ >= 130 && !defined(LOVE_GLSL1_ON_GLSL3)
	#define Texel texture
#else
	#if __VERSION__ >= 130
		#define texture2D Texel
		#define texture3D Texel
		#define textureCube Texel
		#define texture2DArray Texel
		#define love_texture2D texture
		#define love_texture3D texture
		#define love_textureCube texture
		#define love_texture2DArray texture
	#else
		#define love_texture2D texture2D
		#define love_texture3D texture3D
		#define love_textureCube textureCube
		#define love_texture2DArray texture2DArray
	#endif
	vec4 Texel(sampler2D s, vec2 c) { return love_texture2D(s, c); }
	vec4 Texel(samplerCube s, vec3 c) { return love_textureCube(s, c); }
	#if __VERSION__ > 100 || defined(GL_OES_texture_3D)
		vec4 Texel(sampler3D s, vec3 c) { return love_texture3D(s, c); }
	#endif
	#if __VERSION__ >= 130 || defined(GL_EXT_texture_array)
		vec4 Texel(sampler2DArray s, vec3 c) { return love_texture2DArray(s, c); }
	#endif
	#ifdef PIXEL
		vec4 Texel(sampler2D s, vec2 c, float b) { return love_texture2D(s, c, b); }
		vec4 Texel(samplerCube s, vec3 c, float b) { return love_textureCube(s, c, b); }
		#if __VERSION__ > 100 || defined(GL_OES_texture_3D)
			vec4 Texel(sampler3D s, vec3 c, float b) { return love_texture3D(s, c, b); }
		#endif
		#if __VERSION__ >= 130 || defined(GL_EXT_texture_array)
			vec4 Texel(sampler2DArray s, vec3 c, float b) { return love_texture2DArray(s, c, b); }
		#endif
	#endif
	#define texture love_texture
#endif

float gammaToLinearPrecise(float c) {
	return c <= 0.04045 ? c * 0.077399380804954 : pow((c + 0.055) * 0.9478672985782, 2.4);
}
vec3 gammaToLinearPrecise(vec3 c) {
	bvec3 leq = lessThanEqual(c, vec3(0.04045));
	c.r = leq.r ? c.r * 0.077399380804954 : pow((c.r + 0.055) * 0.9478672985782, 2.4);
	c.g = leq.g ? c.g * 0.077399380804954 : pow((c.g + 0.055) * 0.9478672985782, 2.4);
	c.b = leq.b ? c.b * 0.077399380804954 : pow((c.b + 0.055) * 0.9478672985782, 2.4);
	return c;
}
vec4 gammaToLinearPrecise(vec4 c) { return vec4(gammaToLinearPrecise(c.rgb), c.a); }
float linearToGammaPrecise(float c) {
	return c < 0.0031308 ? c * 12.92 : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}
vec3 linearToGammaPrecise(vec3 c) {
	bvec3 lt = lessThanEqual(c, vec3(0.0031308));
	c.r = lt.r ? c.r * 12.92 : 1.055 * pow(c.r, 1.0 / 2.4) - 0.055;
	c.g = lt.g ? c.g * 12.92 : 1.055 * pow(c.g, 1.0 / 2.4) - 0.055;
	c.b = lt.b ? c.b * 12.92 : 1.055 * pow(c.b, 1.0 / 2.4) - 0.055;
	return c;
}
vec4 linearToGammaPrecise(vec4 c) { return vec4(linearToGammaPrecise(c.rgb), c.a); }

// http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1

mediump float gammaToLinearFast(mediump float c) { return c * (c * (c * 0.305306011 + 0.682171111) + 0.012522878); }
mediump vec3 gammaToLinearFast(mediump vec3 c) { return c * (c * (c * 0.305306011 + 0.682171111) + 0.012522878); }
mediump vec4 gammaToLinearFast(mediump vec4 c) { return vec4(gammaToLinearFast(c.rgb), c.a); }

mediump float linearToGammaFast(mediump float c) { return max(1.055 * pow(max(c, 0.0), 0.41666666) - 0.055, 0.0); }
mediump vec3 linearToGammaFast(mediump vec3 c) { return max(1.055 * pow(max(c, vec3(0.0)), vec3(0.41666666)) - 0.055, vec3(0.0)); }
mediump vec4 linearToGammaFast(mediump vec4 c) { return vec4(linearToGammaFast(c.rgb), c.a); }

#define gammaToLinear gammaToLinearFast
#define linearToGamma linearToGammaFast

#ifdef LOVE_GAMMA_CORRECT
	#define gammaCorrectColor gammaToLinear
	#define unGammaCorrectColor linearToGamma
	#define gammaCorrectColorPrecise gammaToLinearPrecise
	#define unGammaCorrectColorPrecise linearToGammaPrecise
	#define gammaCorrectColorFast gammaToLinearFast
	#define unGammaCorrectColorFast linearToGammaFast
#else
	#define gammaCorrectColor
	#define unGammaCorrectColor
	#define gammaCorrectColorPrecise
	#define unGammaCorrectColorPrecise
	#define gammaCorrectColorFast
	#define unGammaCorrectColorFast
#endif
""";

dictionary Vertex = {
	{
		"Header",
		"""
#define love_Position gl_Position

#if __VERSION__ >= 130
	#define attribute in
	#define varying out
	#ifndef LOVE_GLSL1_ON_GLSL3
		#define love_VertexID gl_VertexID
		#define love_InstanceID gl_InstanceID
	#endif
#endif

#ifdef GL_ES
	uniform mediump float love_PointSize;
#endif
"""
	},
	{
		"Functions",
		"""
void setPointSize() {
#ifdef GL_ES
	gl_PointSize = love_PointSize;
#endif
}
		"""
	},
	{
		"Main",
		"""
attribute vec4 VertexPosition;
attribute vec4 VertexTexCoord;
attribute vec4 VertexColor;
attribute vec4 ConstantColor;

varying vec4 VaryingTexCoord;
varying vec4 VaryingColor;

vec4 position(mat4 clipSpaceFromLocal, vec4 localPosition);

void main() {
	VaryingTexCoord = VertexTexCoord;
	VaryingColor = gammaCorrectColor(VertexColor) * ConstantColor;
	setPointSize();
	love_Position = position(ClipSpaceFromLocal, VertexPosition);
}
		"""
	}
};

dictionary Pixel = {
	{
		"Header",
		"""
#ifdef GL_ES
	precision mediump float;
#endif

#define love_MaxCanvases gl_MaxDrawBuffers

#if __VERSION__ >= 130
	#define varying in
	// Some drivers seem to make the pixel shader do more work when multiple
	// pixel shader outputs are defined, even when only one is actually used.
	// TODO: We should use reflection or something instead of this, to determine
	// how many outputs are actually used in the shader code.
	#ifdef LOVE_MULTI_CANVAS
		layout(location = 0) out vec4 love_Canvases[love_MaxCanvases];
		#define love_PixelColor love_Canvases[0]
	#else
		layout(location = 0) out vec4 love_PixelColor;
	#endif
#else
	#ifdef LOVE_MULTI_CANVAS
		#define love_Canvases gl_FragData
	#endif
	#define love_PixelColor gl_FragColor
#endif

// See Shader::updateScreenParams in Shader.cpp.
#define love_PixelCoord (vec2(gl_FragCoord.x, (gl_FragCoord.y * love_ScreenSize.z) + love_ScreenSize.w))
		"""
	},
	{
		"Functions",
		"""
uniform sampler2D love_VideoYChannel;
uniform sampler2D love_VideoCbChannel;
uniform sampler2D love_VideoCrChannel;

vec4 VideoTexel(vec2 texcoords) {
	vec3 yuv;
	yuv[0] = Texel(love_VideoYChannel, texcoords).r;
	yuv[1] = Texel(love_VideoCbChannel, texcoords).r;
	yuv[2] = Texel(love_VideoCrChannel, texcoords).r;
	yuv += vec3(-0.0627451017, -0.501960814, -0.501960814);

	vec4 color;
	color.r = dot(yuv, vec3(1.164,  0.000,  1.596));
	color.g = dot(yuv, vec3(1.164, -0.391, -0.813));
	color.b = dot(yuv, vec3(1.164,  2.018,  0.000));
	color.a = 1.0;

	return gammaCorrectColor(color);
}
		"""
	},
	{
		"Main",
		"""
uniform sampler2D MainTex;
varying LOVE_HIGHP_OR_MEDIUMP vec4 VaryingTexCoord;
varying mediump vec4 VaryingColor;

vec4 effect(vec4 vcolor, Image tex, vec2 texcoord, vec2 pixcoord);

void main() {
	love_PixelColor = effect(VaryingColor, MainTex, VaryingTexCoord.st, love_PixelCoord);
}
		"""
	},
	{
		"MainCustom",
		"""
varying LOVE_HIGHP_OR_MEDIUMP vec4 VaryingTexCoord;
varying mediump vec4 VaryingColor;

void effect();

void main() {
	effect();
}
		"""
	}
};

string DefaultCodeVertex = """
vec4 position(mat4 clipSpaceFromLocal, vec4 localPosition) {
	return clipSpaceFromLocal * localPosition;
}
""";

string DefaultCodePixel = """
vec4 effect(vec4 vcolor, Image tex, vec2 texcoord, vec2 pixcoord) {
	return Texel(tex, texcoord) * vcolor;
}
""";

string DefaultCodeVideoPixel = """
void effect() {
	love_PixelColor = VideoTexel(VaryingTexCoord.xy) * VaryingColor;
}
""";

string DefaultCodeArrayPixel = """
uniform ArrayImage MainTex;
void effect() {
	love_PixelColor = Texel(MainTex, VaryingTexCoord.xyz) * VaryingColor;
}
""";
}

string createShaderStageCode(string stage, string code, string lang, bool gles, bool glsl1on3, bool gammacorrect, bool custom = false, bool multicanvas = false)
{
	string ret;

	if (lang == "glsl1") {
		if (gles) {
			ret += "#version 100";
		} else {
			ret += "#version 120";
		}
	} else if (lang == "glsl3") {
		if (gles) {
			ret += "#version 300 es";
		} else {
			ret += "#version 330 core";
		}
	}
	ret += "\n";

	ret += "#define " + stage + " " + stage + "\n";

	if (glsl1on3) {
		ret += "#define LOVE_GLSL1_ON_GLSL3 1\n";
	}

	if (gammacorrect) {
		ret += "#define LOVE_GAMMA_CORRECT 1\n";
	}

	if (multicanvas) {
		ret += "#define LOVE_MULTI_CANVAS 1\n";
	}

	ret += GLSL::Syntax;

	if (stage == "VERTEX") {
		ret += string(GLSL::Vertex["Header"]);
	} else if (stage == "PIXEL") {
		ret += string(GLSL::Pixel["Header"]);
	}

	ret += GLSL::Uniforms;
	ret += GLSL::Functions;

	if (stage == "VERTEX") {
		ret += string(GLSL::Vertex["Functions"]);
	} else if (stage == "PIXEL") {
		ret += string(GLSL::Pixel["Functions"]);
	}

	if (stage == "VERTEX") {
		if (custom && GLSL::Vertex.exists("MainCustom")) {
			ret += string(GLSL::Vertex["MainCustom"]);
		} else {
			ret += string(GLSL::Vertex["Main"]);
		}
	} else if (stage == "PIXEL") {
		if (custom && GLSL::Pixel.exists("MainCustom")) {
			ret += string(GLSL::Pixel["MainCustom"]);
		} else {
			ret += string(GLSL::Pixel["Main"]);
		}
	}

	if ((lang == "glsl1" || glsl1on3) && !gles) {
		ret += "#line 0\n";
	} else {
		ret += "#line 1\n";
	}

	ret += code;

	return ret;
}

class LanguageType
{
	string m_lang;
	string m_target;
	bool m_gles;

	LanguageType(const string &in lang, const string &in target, bool gles)
	{
		m_lang = lang;
		m_target = target;
		m_gles = gles;
	}
}

dictionary makeLanguageTable(const LanguageType &info, bool gammacorrect)
{
	dictionary ret = {
		{ "vertex", createShaderStageCode("VERTEX", GLSL::DefaultCodeVertex, info.m_target, info.m_gles, false, gammacorrect) },
		{ "pixel", createShaderStageCode("PIXEL", GLSL::DefaultCodePixel, info.m_target, info.m_gles, false, gammacorrect) },
		{ "videopixel", createShaderStageCode("PIXEL", GLSL::DefaultCodeVideoPixel, info.m_target, info.m_gles, false, gammacorrect, true) },
		{ "arraypixel", createShaderStageCode("PIXEL", GLSL::DefaultCodeArrayPixel, info.m_target, info.m_gles, false, gammacorrect, true) }
	};
	return ret;
}

void angel_graphics_init()
{
	dictionary defaults;
	dictionary defaults_gammacorrect;

	array<LanguageType@> langs;
	langs.insertLast(LanguageType("glsl1", "glsl1", false));
	langs.insertLast(LanguageType("essl1", "glsl1", true));
	langs.insertLast(LanguageType("glsl3", "glsl3", false));
	langs.insertLast(LanguageType("essl3", "glsl3", true));

	for (uint i = 0; i < langs.length(); i++) {
		auto info = langs[i];
		defaults[info.m_lang] = makeLanguageTable(info, false);
		defaults_gammacorrect[info.m_lang] = makeLanguageTable(info, true);
	}

	angel::graphics::_setDefaultShaderCode(defaults, defaults_gammacorrect);
	angel::graphics::setGammaCorrect(true);
}
