@vs vs
in vec4 position;
in vec4 color0;
in float brightness0;

out vec4 color;
out float brightness;

void main() {
    gl_Position = position;
    color = color0;
    brightness = brightness0;
}
@end

@fs fs
in vec4 color;
in float brightness;
out vec4 frag_color;

void main() {
    frag_color = color * brightness;
}
@end

@program triangle vs fs
