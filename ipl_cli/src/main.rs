use std::env;
use std::fs;
use std::path::Path;
use std::process;

mod commands;
mod project;
mod package;
mod compiler;

use commands::{Command, parse_args};
use project::ProjectManager;
use package::PackageManager;
use compiler::Compiler;

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() < 2 {
        print_usage();
        return;
    }
    
    let command = match parse_args(&args) {
        Ok(cmd) => cmd,
        Err(e) => {
            eprintln!("Error: {}", e);
            print_usage();
            return;
        }
    };
    
    let mut project_manager = ProjectManager::new();
    let mut package_manager = PackageManager::new();
    let compiler = Compiler::new();
    
    match command {
        Command::Create { name, org, description } => {
            match project_manager.create_project(&name, &org, &description) {
                Ok(_) => {
                    println!("Successfully created project '{}'", name);
                    println!("Project location: {}", name);
                    println!("\nTo get started:");
                    println!("  cd {}", name);
                    println!("  ipl get          # Get dependencies");
                    println!("  ipl run          # Run the project");
                },
                Err(e) => eprintln!("Failed to create project: {}", e)
            }
        },
        Command::Get => {
            match package_manager.get_dependencies() {
                Ok(_) => println!("Dependencies updated successfully"),
                Err(e) => eprintln!("Failed to get dependencies: {}", e)
            }
        },
        Command::Run => {
            match compiler.run() {
                Ok(_) => {},
                Err(e) => eprintln!("Failed to run project: {}", e)
            }
        },
        Command::Build { target } => {
            match compiler.build(&target) {
                Ok(_) => {
                    println!("Build completed successfully");
                    if let Some(t) = target {
                        println!("Target: {}", t);
                    }
                },
                Err(e) => eprintln!("Failed to build project: {}", e)
            }
        },
        Command::Clean => {
            match compiler.clean() {
                Ok(_) => println!("Build artifacts cleaned successfully"),
                Err(e) => eprintln!("Failed to clean build artifacts: {}", e)
            }
        },
        Command::Add { package } => {
            match package_manager.add_dependency(&package) {
                Ok(_) => println!("Added dependency: {}", package),
                Err(e) => eprintln!("Failed to add dependency: {}", e)
            }
        },
        Command::Remove { package } => {
            match package_manager.remove_dependency(&package) {
                Ok(_) => println!("Removed dependency: {}", package),
                Err(e) => eprintln!("Failed to remove dependency: {}", e)
            }
        },
        Command::List => {
            match package_manager.list_dependencies() {
                Ok(deps) => {
                    println!("Project dependencies:");
                    for dep in deps {
                        println!("  - {}", dep);
                    }
                },
                Err(e) => eprintln!("Failed to list dependencies: {}", e)
            }
        },
        Command::Help => {
            print_usage();
        },
        Command::Version => {
            println!("IPL Command Line Manager v0.1.0");
        }
    }
}

fn print_usage() {
    println!("IPL - Impossible Programming Language Command Line Interface");
    println!();
    println!("Usage: ipl <command> [arguments]");
    println!();
    println!("Global options:");
    println!("  -h, --help     Print this usage information.");
    println!("  -V, --version  Print the application version.");
    println!();
    println!("Available commands:");
    println!("  create <project_name>    Create a new IPL project");
    println!("  get                      Get project dependencies");
    println!("  run                      Run the current project");
    println!("  build [target]           Build the project for specified target");
    println!("  clean                    Clean build artifacts");
    println!("  add <package>            Add a dependency");
    println!("  remove <package>         Remove a dependency");
    println!("  list                     List all dependencies");
    println!("  help                     Print this help message");
    println!();
    println!("Run 'ipl help <command>' for more information about a command.");
}
