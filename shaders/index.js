var glsl = require('glslify');
var fs = require('fs-extra');
var child_process = require('child_process');
var path = require("path");
var chalk = require("chalk");
var combinatronics = require('js-combinatorics');


const supported_ext = ['.vert', '.tesc', '.tese', '.geom', '.frag', '.comp'];

const f3d_constants =  {
	'F3D_COLOR_AMBIENT' : 0x02,
	'F3D_COLOR_DIFFUSE' : 0x04,
	'F3D_COLOR_SPECULAR' : 0x08,
	'F3D_LIGHT_DIRECTIONAL' : 0x20,
	'F3D_LIGHT_POINT' : 0x40,
	'F3D_LIGHT_SPOT' : 0x80,
	'F3D_UNIFORM_CAMERA' : 0x100,
	'F3D_UNIFORM_MODEL' : 0x200,
	'F3D_UNIFORM_MATERIAL' : 0x400,
	'F3D_UNIFORM_LIGHT' : 0x800,
	'F3D_ATTR_POSITION' : 0x100000,
	'F3D_ATTR_NORMAL' : 0x200000,
	'F3D_VULKAN_TRANSFORM_Y': 0x8000000,
	'F3D_VULKAN_TRANSFORM_Z': 0x4000000
}; 

const f3d_shader_interface = {
	'F3D_UNIFORM_UNDEFINED': 0x0,
	// 'F3D_UNIFORM_SAMPLER_AMBIENT' : 0x1000,
	// 'F3D_UNIFORM_SAMPLER_DIFFUSE' : 0x2000,
	// 'F3D_UNIFORM_SAMPLER_SPECULAR' : 0x4000,
	// 'F3D_UNIFORM_SAMPLER_EMISSIVE' : 0x8000,
	// 'F3D_UNIFORM_SAMPLER_HEIGHT' : 0x10000,
	// 'F3D_UNIFORM_SAMPLER_NORMALS' : 0x20000,
	/*
	'F3D_UNIFORM_RESERVED_1 = 0x40000,
	'F3D_UNIFORM_RESERVED_2 = 0x80000,
	*/
	// 'F3D_ATTR_COLOR' : 0x400000,
	//'F3D_ATTR_UV' : 0x800000
	/*
	'F3D_COLOR_EMISSIVE' : 0x10,
	F3D_ATTR_RESERVED_1 = 0x1000000,
	F3D_ATTR_RESERVED_2 = 0x2000000,
	F3D_VULKAN_TRANSFORM_W = 0x4000000,
	*/
}

const f3d_diffuse_shading_type = {
 //   'F3D_SHADING_DIFFUSE_FLAT': 0x2,
    'F3D_SHADING_DIFFUSE_LAMBERT': 0x04,
//    'F3D_SHADING_DIFFUSE_ORENNAYAR': 0x08,
 //   'F3D_SHADING_DIFFUSE_TOON': 0x010
};
const f3d_specular_shading_type = {
 //   'F3D_SHADING_SPECULAR_PHONG': 0x100,
    'F3D_SHADING_SPECULAR_BLINN_PHONG': 0x200,
//    'F3D_SHADING_SPECULAR_BECKMANN': 0x400,
//    'F3D_SHADING_SPECULAR_COOK_TORRANCE': 0x800,
 //   'F3D_SHADING_SPECULAR_WARD': 0x1000,
//    'F3D_SHADING_SPECULAR_GAUSSIAN': 0x2000
};

function generateDefinesCombination(enum_map) {
    var result = [];
    var combinations = combinatronics.power(Object.keys(enum_map));
    combinations.forEach(function(combination) {
        var mask = 0x00;
        if (combination.length > 0) {
            combination.forEach(function(elem) {
                mask |= enum_map[elem];
            });
            result.push(mask);
        }
    });
    return result;
}
function textFromMask(enum_map, mask) {
    var text = '';
    for (k in enum_map) {
        if ((enum_map[k] & mask) === enum_map[k])
            text += '#define ' + k + ' 0x' + enum_map[k].toString(16) + '\r\n';
    }
    return text;
}

function generateGLSL(srcDir, destDir) {
	//Read input folder and create output folder
	var input_files = fs.readdirSync(srcDir);
	if (fs.existsSync(destDir) === false)
		fs.mkdirSync(destDir);
	
	console.log(`Processing ${input_files.length} files from ${chalk.cyan(srcDir)} to ${chalk.cyan(destDir)}`);
	
	var constant_def = '';
	var constant_mask = 0x00;
	for (var key in f3d_constants) {
	    constant_def += '#define ' + key + ' 0x' + f3d_constants[key].toString(16) + '\r\n';
		constant_mask |= f3d_constants[key];
	}
	var shader_interface = generateDefinesCombination(f3d_shader_interface);

	for (var i = 0; i < input_files.length; i++) {
	
		try {
			//Compute file paths
		    var input_file = input_files[i];
		    var input_extension = path.extname(input_files[i]);

		    if (supported_ext.includes(input_extension)) // Supported extension ONLY
		    {
		        //Process glsl using glslify
		        var output_code = glsl.file(input_file, { basedir: srcDir });

		        for (var d = 0; d < Object.keys(f3d_diffuse_shading_type).length; d++) {
		            var diffuse_shading_mask = Object.values(f3d_diffuse_shading_type)[d]; // After the interface 32 bits
		            var diffuse_shading_text = textFromMask(f3d_diffuse_shading_type, diffuse_shading_mask);

		            for (var s = 0; s < Object.keys(f3d_specular_shading_type).length; s++) {
		                var specular_shading_mask = Object.values(f3d_specular_shading_type)[s]; //Afeter the inteface 32 bits
		                var specular_shading_text = textFromMask(f3d_specular_shading_type, specular_shading_mask);
		                shader_interface.forEach(function (interface_def) {
		                    var obj = {
		                        text: constant_def + textFromMask(f3d_shader_interface, interface_def) + diffuse_shading_text + specular_shading_text,
		                        interface_mask: interface_def | constant_mask,
		                        shading_mask: specular_shading_mask | diffuse_shading_mask,
		                    }

		                    var output_name = obj.interface_mask.toString(16);
		                    while (output_name.length < 8)
		                        output_name = '0' + output_name;
		                    output_name = obj.shading_mask.toString(16) + output_name;
		                    while (output_name.length < 16)
		                        output_name = '0' + output_name;
		                    output_name = '0x' + output_name + input_extension;
		                    var output_path = path.join(destDir, output_name);

		                    //Remplace in the code the generated macros
		                    var code = output_code.replace('#define F3D_REPLACED_DECLARATIONS', obj.text);

		                    //Write file
		                    fs.writeFileSync(output_path, code);
		                    console.log(`${chalk.green("GLSL generation success")} [${chalk.cyan(output_path)}]`);
		                });
		            } // Specular shading
		        } //Diffuse shading

		    }// if supported ext
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