var glsl = require('glslify');
var fs = require('fs-extra');
var child_process = require('child_process');
var path = require("path");
var chalk = require("chalk");


const f3d_constants =  {
    'F3D_VULKAN_TRANSFORM_Y': 0x01
};

const f3d_attributes = {
    'F3D_ATTR_POSITION' : 0x01, 
    'F3D_ATTR_NORMAL': 0x02,
    'F3D_ATTR_COLOR': 0x04,
    'F3D_ATTR_UV': 0x08
};
const f3d_uniforms = {
    'F3D_UNIFORM_CAMERA': 0x01,
    'F3D_UNIFORM_MODEL': 0x02,
    'F3D_UNIFORM_MATERIAL': 0x04,
    'F3D_UNIFORM_LIGHT': 0x08,
    'F3D_UNIFORM_SAMPLER_AMBIENT': 0x10,
    'F3D_UNIFORM_SAMPLER_DIFFUSE': 0x20,
    'F3D_UNIFORM_SAMPLER_SPECULAR': 0x40,
    'F3D_UNIFORM_SAMPLER_EMISSIVE': 0x80,
    'F3D_UNIFORM_SAMPLER_HEIGHT': 0x100,
    'F3D_UNIFORM_SAMPLER_NORMALS': 0x200,
    'F3D_UNIFORM_SAMPLER_DISPLACEMENT': 0x400,
    'F3D_UNIFORM_SAMPLER_REFLECTION': 0x800
};

const f3d_color_type = {
    'F3D_COLOR_AMBIENT': 0x01,
    'F3D_COLOR_DIFFUSE': 0x02,
    'F3D_COLOR_SPECULAR': 0x04,
    'F3D_COLOR_EMISSIVE': 0x08,
    'F3D_COLOR_REFLECTIVE': 0x10
};
const f3d_light_type =  {
    'F3D_LIGHT_DIRECTIONAL': 0x01,
    'F3D_LIGHT_POINT': 0x02,
    'F3D_LIGHT_SPOT': 0x04
};
const f3d_diffuse_shading_type = {
    'F3D_SHADING_DIFFUSE_FLAT': 0x01,
    'F3D_SHADING_DIFFUSE_GOURAUD': 0x02,
    'F3D_SHADING_DIFFUSE_LAMBERT': 0x04,
    'F3D_SHADING_DIFFUSE_ORENNAYAR': 0x08,
    'F3D_SHADING_DIFFUSE_TOON': 0x010,
};
const f3d_specular_shading_type = {
    'F3D_SHADING_SPECULAR_PHONG': 0x01,
    'F3D_SHADING_SPECULAR_BLINN_PHONG': 0x02,
    'F3D_SHADING_SPECULAR_BECKMANN': 0x04,
    'F3D_SHADING_SPECULAR_COOK_TORRANCE': 0x08,
    'F3D_SHADING_SPECULAR_WARD': 0x010,
    'F3D_SHADING_SPECULAR_GAUSSIAN': 0x020
};

const supported_ext = [ '.vert', '.tesc', '.tese', '.geom', '.frag', '.comp'];

function generateDefinesCombination(input_macro_values) {
    var r_fn = function()
}

function generateGLSL(srcDir, destDir) {
	//Read input folder and create output folder
	var input_files = fs.readdirSync(srcDir);
	if (fs.existsSync(destDir) === false)
		fs.mkdirSync(destDir);
	
	console.log(`Processing ${input_files.length} files from ${chalk.cyan(srcDir)} to ${chalk.cyan(destDir)}`);
	
	var constant_defines_code = "";
	for (var key in f3d_constants) {
	    var def = "#define " + key + " " + f3d_constants[key].toString(16) + "\n";
	    constant_defines_code += def;
	}

	for (var i = 0; i < input_files.length; i++) {
	
		try {
			//Compute file paths
			var input_file = input_files[i];
			var output_name = path.basename(input_file);
			var output_path = path.join(destDir, output_name);
			
			//Process glsl using glslify
			var output_code = glsl.file(input_file, { basedir: srcDir });

            //TODO: GENERATE MACROS HERE
            //Adding preprocessor code
			output_code =
                constant_defines_code +
                output_code;

		    //Write result to file
			fs.writeFileSync(output_path, output_code);
			console.log(`${chalk.green("GLSL generation success")} [${chalk.cyan(output_path)}]`);
		}
		catch (error) {
			console.log(error);
			console.log(`${chalk.red("GLSL generation error")} [${chalk.cyan(output_path)}] : ${error.stdout}`);
		}
	}
}

function generateSPIRV(srcDir, destDir) {
	//Read input folder and create output folder
	var input_files = fs.readdirSync(srcDir);
	if (fs.existsSync(destDir) === false)
		fs.mkdirSync(destDir);

	console.log(`Processing ${input_files.length} files from ${chalk.cyan(srcDir)} to ${chalk.cyan(destDir)}`);

	for (var i = 0; i < input_files.length; i++) {
		try {
			//Compute files paths
			var input_file = path.join(srcDir, input_files[i]);
			var input_ext = path.extname(input_file);
			var output_name = path.basename(input_file) + ".spv";
			var output_path = path.join(destDir, output_name);
		
			//If is supported by spirv compiler
			if (supported_ext.includes(input_ext))
			{
				//Compile to spirv
				var command_line = `glslangValidator -V ${input_file} -o ${output_path}`;
				child_process.execSync(command_line);
				console.log(`${chalk.green("SPIRV generation success")} [${chalk.cyan(output_path)}]`);
			}
		}
		catch (error) {
			console.log(`${chalk.red("SPIRV generation error")} [${chalk.cyan(output_path)}] : ${error.stdout}`);
		}
	}
}

generateGLSL("./src", "./gen");
generateSPIRV("./gen", "./bin");