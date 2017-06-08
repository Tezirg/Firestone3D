for shader in *.frag *.vert ;
	do
		glslangValidator -V $shader -o $shader.spv
	done
read over