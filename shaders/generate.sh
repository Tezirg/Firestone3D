for shader in *.vert *.frag;
	do
		glslangValidator -V $shader -o $shader.spv
	done
read over