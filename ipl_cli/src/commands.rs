use std::fmt;

#[derive(Debug)]
pub enum Command {
    Create { name: String, org: String, description: String },
    Get,
    Run,
    Build { target: Option<String> },
    Clean,
    Add { package: String },
    Remove { package: String },
    List,
    Help,
    Version,
}

#[derive(Debug)]
pub enum CliError {
    MissingArgument(String),
    InvalidArgument(String),
    UnknownCommand(String),
}

impl fmt::Display for CliError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            CliError::MissingArgument(arg) => write!(f, "Missing argument: {}", arg),
            CliError::InvalidArgument(arg) => write!(f, "Invalid argument: {}", arg),
            CliError::UnknownCommand(cmd) => write!(f, "Unknown command: {}", cmd),
        }
    }
}

impl std::error::Error for CliError {}

pub fn parse_args(args: &[String]) -> Result<Command, CliError> {
    if args.len() < 2 {
        return Err(CliError::MissingArgument("command".to_string()));
    }

    match args[1].as_str() {
        "create" => {
            if args.len() < 3 {
                return Err(CliError::MissingArgument("project_name".to_string()));
            }
            
            let name = args[2].clone();
            let org = args.get(3).cloned().unwrap_or_else(|| "com.example".to_string());
            let description = args.get(4).cloned().unwrap_or_else(|| "A new IPL project".to_string());
            
            Ok(Command::Create { name, org, description })
        },
        "get" => Ok(Command::Get),
        "run" => Ok(Command::Run),
        "build" => {
            let target = if args.len() > 2 {
                Some(args[2].clone())
            } else {
                None
            };
            Ok(Command::Build { target })
        },
        "clean" => Ok(Command::Clean),
        "add" => {
            if args.len() < 3 {
                return Err(CliError::MissingArgument("package".to_string()));
            }
            Ok(Command::Add { package: args[2].clone() })
        },
        "remove" => {
            if args.len() < 3 {
                return Err(CliError::MissingArgument("package".to_string()));
            }
            Ok(Command::Remove { package: args[2].clone() })
        },
        "list" => Ok(Command::List),
        "help" | "-h" | "--help" => Ok(Command::Help),
        "version" | "-V" | "--version" => Ok(Command::Version),
        cmd => Err(CliError::UnknownCommand(cmd.to_string())),
    }
}