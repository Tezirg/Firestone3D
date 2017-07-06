var glsl = require('glslify');
var fs = require('fs-extra');
var child_process = require('child_process');
var path = require("path");
var chalk = require("chalk");

function generateGLSL(srcDir, destDir) {
	//Read input folder and create output folder
	var input_files = fs.readdirSync(srcDir);
	if (fs.existsSync(destDir) === false)
		fs.mkdirSync(destDir);
	
	console.log(`Processing ${input_files.length} files from ${chalk.cyan(srcDir)} to ${chalk.cyan(destDir)}`);
	
	for (var i = 0; i < input_files.length; i++) {
	
		try {
			//Compute file paths
			var input_file = input_files[i];
			var output_name = path.basename(input_files[i]);
			var output_path = path.join(destDir, output_name);
		
			//Process glsl using glslify
			var output_code = glsl.file(input_file, { basedir: srcDir });

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
			var output_name = path.basename(input_file) + ".spv";
			var output_path = path.join(destDir, output_name);
			
			//Compile to spirv
			var command_line = `glslangValidator.exe -V ${input_file} -o ${output_path}`;
			child_process.execSync(command_line);
			console.log(`${chalk.green("SPIRV generation success")} [${chalk.cyan(output_path)}]`);
		}
		catch (error) {
			console.log(`${chalk.red("SPIRV generation error")} [${chalk.cyan(output_path)}] : ${error.stdout}`);
		}
	}
}

generateGLSL("./src", "./gen");
generateSPIRV("./gen", "./bin");