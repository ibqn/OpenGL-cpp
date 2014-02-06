#version 130 

precision mediump float;
uniform vec3 vColor;

void main () {
	gl_FragColor = vec4 (vColor, 1);
}
