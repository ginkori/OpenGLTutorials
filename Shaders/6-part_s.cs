#version 440 core
layout (local_size_x = 11) in;

layout (location = 0) uniform vec3 BlackHolePos1;
layout (location = 1) uniform vec3 BlackHolePos2;
uniform float Gravity1 = 10.0;
uniform float Gravity2 = 10.0;
uniform float ParticleInvMass = 1.0 / 0.1;
uniform float DeltaT = 0.005;

layout (std430, binding=0) buffer Pos {
vec4 Position[];
};

layout(std430, binding=1) buffer Vel {
vec4 Velocity[];
};

void main() {
uint idx = gl_GlobalInvocationID.x;
vec3 p = Position[idx].xyz;
vec3 v = Velocity[idx].xyz;
//
vec3 d = BlackHolePos1 - p;
vec3 force = (Gravity1 / length(d)) * normalize(d);
//
d = BlackHolePos2 - p;
force += (Gravity2 / length(d)) * normalize(d);
	
// Метод Эйлера
vec3 a = force * ParticleInvMass;
Position[idx] = vec4(p + v * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
Velocity[idx] = vec4( v + a * DeltaT, 0.0);

}