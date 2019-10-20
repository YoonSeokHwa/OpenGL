//--- vertex shader: vertex.glvs

#version 330 
// in_Position was bound to attribute index 0 and 
// in_Color was bound to attribute index 1

in  vec3 in_Position; // ��ġ�Ӽ� 
in  vec3 in_Color; // ����Ӽ�
out vec3 ex_Color; // �����׸�Ʈ���̴���������

void main(void) { 
	gl_Position = vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);
	ex_Color = in_Color;
}
