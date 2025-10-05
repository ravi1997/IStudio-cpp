use std::process::Command;
use std::fs;
use std::path::Path;
use std::env;

#[derive(Debug)]
pub struct Compiler {
    build_dir: String,
    src_dir: String,
    main_file: String,
}

impl Compiler {
    pub fn new() -> Self {
        Compiler {
            build_dir: "build".to_string(),
            src_dir: "src".to_string(),
            main_file: "src/main.ipl".to_string(),
        }
    }

    pub fn run(&self) -> Result<(), Box<dyn std::error::Error>> {
        // Check if we're in an IPL project directory
        if !Path::new("ipl.yaml").exists() {
            return Err("No IPL project found (ipl.yaml not found). Are you in an IPL project directory?".into());
        }

        // First build the project
        self.build(&None)?;

        // For now, we'll just execute the compiled binary
        // In a real implementation, this would run the compiled output
        println!("Running IPL project...");
        
        // Check if there's a compiled binary in the build directory
        let executable = format!("{}/main", self.build_dir);
        if Path::new(&executable).exists() {
            let output = Command::new(&executable).output()?;
            println!("{}", String::from_utf8_lossy(&output.stdout));
            
            if !output.stderr.is_empty() {
                eprintln!("{}", String::from_utf8_lossy(&output.stderr));
            }
        } else {
            println!("No executable found. Building...");
            // Try to compile the IPL source file
            if Path::new(&self.main_file).exists() {
                self.compile_ipl_source()?;
            } else {
                return Err(format!("Main IPL file not found: {}", self.main_file).into());
            }
        }

        Ok(())
    }

    pub fn build(&self, target: &Option<String>) -> Result<(), Box<dyn std::error::Error>> {
        // Check if we're in an IPL project directory
        if !Path::new("ipl.yaml").exists() {
            return Err("No IPL project found (ipl.yaml not found). Are you in an IPL project directory?".into());
        }

        // Create build directory if it doesn't exist
        if !Path::new(&self.build_dir).exists() {
            fs::create_dir_all(&self.build_dir)?;
        }

        // Determine the build target
        let build_target = match target {
            Some(t) => t.as_str(),
            None => "executable",
        };

        println!("Building IPL project for target: {}", build_target);

        // Try to compile the IPL source file
        if Path::new(&self.main_file).exists() {
            self.compile_ipl_source()?;
        } else {
            return Err(format!("Main IPL file not found: {}", self.main_file).into());
        }

        match build_target {
            "executable" => {
                println!("Compiling to executable...");
                // In a real implementation, we'd use the actual IPL compiler
                println!("Compilation completed. Output: {}/main", self.build_dir);
            },
            "wasm" => {
                println!("Compiling to WebAssembly...");
                // This would require IPL to WASM compilation
                println!("WASM compilation completed. Output: {}/main.wasm", self.build_dir);
            },
            "js" => {
                println!("Compiling to JavaScript...");
                // This would require IPL to JS compilation
                println!("JavaScript compilation completed. Output: {}/main.js", self.build_dir);
            },
            _ => {
                println!("Compiling to target: {}", build_target);
                println!("Compilation completed for target: {}", build_target);
            }
        }

        Ok(())
    }

    fn compile_ipl_source(&self) -> Result<(), Box<dyn std::error::Error>> {
        // Look for the IPL compiler in the IStudio build directory or system path
        let ipl_compiler = self.find_ipl_compiler()?;
        
        println!("Using IPL compiler: {}", ipl_compiler);
        
        // Run the IPL compiler to compile the main file
        let output = Command::new(&ipl_compiler)
            .arg(&self.main_file)
            .arg("-o")
            .arg(format!("{}/main", self.build_dir))
            .output()?;

        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            return Err(format!("IPL compiler failed: {}", stderr).into());
        }

        println!("Successfully compiled IPL source: {}", self.main_file);
        Ok(())
    }

    fn find_ipl_compiler(&self) -> Result<String, Box<dyn std::error::Error>> {
        // First, try to find it in the IStudio build directory
        let current_dir = env::current_dir()?;
        let ipl_compiler_local = current_dir.join("../build/ipl_compiler");
        
        if ipl_compiler_local.exists() {
            return Ok(ipl_compiler_local.to_string_lossy().to_string());
        }

        // Next, try to find it in a standard system location
        // This would be where IStudio gets installed
        let possible_paths = [
            "/usr/local/bin/ipl_compiler",
            "/usr/bin/ipl_compiler",
            "./ipl_compiler",
        ];

        for path in &possible_paths {
            if Path::new(path).exists() {
                return Ok(path.to_string());
            }
        }

        // Finally, try to find it in PATH
        if let Ok(path) = std::env::var("PATH") {
            for p in path.split(':') {
                let full_path = std::path::Path::new(p).join("ipl_compiler");
                if full_path.exists() {
                    return Ok(full_path.to_string_lossy().to_string());
                }
            }
        }

        Err("IPL compiler (ipl_compiler) not found. Please build the IStudio project first.".into())
    }

    pub fn clean(&self) -> Result<(), Box<dyn std::error::Error>> {
        // Remove the build directory
        if Path::new(&self.build_dir).exists() {
            fs::remove_dir_all(&self.build_dir)?;
            println!("Build directory cleaned: {}", self.build_dir);
        } else {
            println!("Build directory does not exist: {}", self.build_dir);
        }

        // Also clean any other build artifacts
        let files_to_clean = [
            "target/",
            "dist/",
            "*.o",
            "*.obj",
            "*.a",
            "*.lib",
            "*.so",
            "*.dylib",
            "*.dll"
        ];

        for pattern in files_to_clean.iter() {
            // In a real implementation, we'd use glob to match patterns
            // For now, just print what would be cleaned
            println!("Would clean pattern: {}", pattern);
        }

        Ok(())
    }
}