use clap::{Parser, ValueEnum, arg, builder::ValueParser};
use pyo3::pyclass;
use std::{
    error::Error,
    fmt::Display,
    path::PathBuf,
    sync::{LazyLock, RwLock},
};

use crate::{
    fuzz::fuzzbase::Fuzzer,
    ice_process,
};

#[pyclass]
#[derive(PartialEq, PartialOrd, Clone, Copy, Debug, ValueEnum)]
pub enum LogLevel {
    Error = 0,
    Warn = 1,
    Info = 2,
    Debug = 3,
}

#[derive(ValueEnum, Clone, Debug)]
pub enum JobType {
    Gen,
    Mask,
    Infill,
    Fuzz,
    Dump,
}

#[derive(ValueEnum, Clone, Debug)]
pub enum FuzzerType {
    Dummy,
    Load,
    Splicer,
    Syn,
    NodeMutate,
    LLAMA,
    LLM,
    NoOp,
}
impl FuzzerType {
    pub fn new_fuzzer(&self, args: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        match self {
            FuzzerType::Dummy => Ok(crate::fuzz::fuzzbase::DummyFuzzer::new(args)?),
            FuzzerType::Load => Ok(crate::fuzz::fuzzbase::LoadFuzzer::new(args)?),
            FuzzerType::Splicer => Ok(crate::strategies::splicer::SplicerFuzzer::new(args)?),
            FuzzerType::Syn => Ok(crate::strategies::syn::SynFuzzer::new(args)?),
            FuzzerType::NodeMutate => Ok(crate::strategies::synmutate::NodeMutater::new(args)?),
            FuzzerType::LLAMA => Ok(crate::strategies::models::llama_cpp::LlamaCppFuzzer::new(
                args,
            )?),
            FuzzerType::LLM => Ok(crate::strategies::models::python_api::LLMModule::new(args)?),
            FuzzerType::NoOp => Err("NoOp should not be parsed.".into()),
        }
    }
}
fn fuzzerjob_help() -> String {
    let possible_fuzzers = FuzzerType::value_variants().iter().map(|f| {
        let binding = f.to_possible_value()
            .expect("ValueEnum::value_variants contains only values with a corresponding ValueEnum::to_possible_value");
        let s = binding
            .get_name_and_aliases();
        s.collect::<Vec<_>>().join("/")
    }).collect::<Vec<_>>().join(", ");
    let possible_jobs = JobType::value_variants().iter().map(|f| {
        let binding = f.to_possible_value()
            .expect("ValueEnum::value_variants contains only values with a corresponding ValueEnum::to_possible_value");
        let s = binding
            .get_name_and_aliases();
        s.collect::<Vec<_>>().join("/")
    }).collect::<Vec<_>>().join(", ");
    format!(
        "Fuzzer: \n\t{}\n\n Job: \n\t{}\n",
        possible_fuzzers, possible_jobs
    )
}

#[derive(Clone, Debug)]
pub struct FuzzerJob {
    pub fuzzer: FuzzerType,
    pub job: JobType,
}
impl FuzzerJob {
    pub fn parser(s: &str) -> Result<FuzzerJob, Box<dyn Error + Send + Sync>> {
        let mut parts = s.split(':');
        let fuzzer = parts.next().ok_or("Missing fuzzer type")?;
        let job = parts.next().ok_or("Missing job type")?;
        let fuzzer_type = FuzzerType::from_str(fuzzer, true).map_err(|_| "Invalid fuzzer type")?;
        let job_type = JobType::from_str(job, true).map_err(|_| "Invalid job type")?;
        Ok(FuzzerJob {
            fuzzer: fuzzer_type,
            job: job_type,
        })
    }
}

#[derive(ValueEnum, Clone, Debug)]
pub enum FilterJob {
    Dummy,
    QueryStack,
    PanicFunc,
}
impl Display for FilterJob {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            FilterJob::Dummy => write!(f, "dummy"),
            FilterJob::QueryStack => write!(f, "query-stack"),
            FilterJob::PanicFunc => write!(f, "panic-func"),
        }
    }
}
impl FilterJob {
    pub fn new_filter(&self, _: &Args) -> Result<Box<dyn ice_process::ICEFilter>, Box<dyn Error>> {
        match self {
            FilterJob::Dummy => Ok(crate::ice_process::DummyFilter::new()),
            FilterJob::QueryStack => Ok(crate::ice_process::querystack::QueryStackFilter::new()),
            FilterJob::PanicFunc => Ok(crate::ice_process::panicfunc::PanicFuncFilter::new()),
        }
    }
}

#[pyclass]
#[derive(Parser, Debug, Clone)]
pub struct Args {
    #[arg(long, value_name = "RUSTC_STAGE")]
    #[pyo3(get)]
    pub stage: Option<String>,

    #[arg(short, long, value_name = "DIR")]
    #[pyo3(get)]
    pub input: Vec<PathBuf>,

    #[arg(short, long, value_name = "DIR")]
    #[arg(default_value = "out")]
    #[pyo3(get)]
    pub output: PathBuf,

    #[arg(short, long, value_name = "DIR")]
    #[arg(default_value = ".")]
    #[pyo3(get)]
    pub datas: PathBuf,

    #[arg(short = 'j', value_name = "THREAD")]
    #[arg(default_value = "1")]
    #[pyo3(get)]
    pub nthread: usize,

    #[arg(long)]
    #[arg(default_value = "true")]
    #[pyo3(get)]
    pub skip_hang: bool,

    #[arg(short, long, value_name = "FILE/NAME")]
    #[arg(default_value = "model.gguf")]
    #[pyo3(get)]
    pub model: String,

    #[arg(long)]
    #[arg(default_value = "false")]
    #[pyo3(get)]
    pub gpu: bool,

    #[arg(long = "mj", value_name = "THREAD")]
    #[arg(default_value = "10")]
    #[pyo3(get)]
    pub model_nthread: usize,

    #[arg(long = "log", value_name = "LEVEL")]
    #[arg(default_value = "info")]
    #[pyo3(get)]
    pub log_level: LogLevel,

    #[arg(short, long, value_name = "LOOP")]
    #[arg(default_value = "1")]
    #[pyo3(get)]
    pub _loopcnt: usize,

    #[arg(long = "inf", value_name = "LOOP", conflicts_with = "_loopcnt")]
    #[arg(default_value = "false")]
    #[pyo3(get)]
    pub _infloop: bool,

    #[arg(short, long, value_name = "FILTER")]
    #[arg(default_values_t = Vec::from([FilterJob::QueryStack, FilterJob::PanicFunc]))]
    pub filters: Vec<FilterJob>,

    // Positional arguments

    // Define the jobs used in the program
    // given as a list of (FuzzerType, JobType)
    #[arg(value_name = "JOBS", num_args = 1..)]
    #[arg(value_parser = ValueParser::new(FuzzerJob::parser))]
    // help message
    #[arg(help = fuzzerjob_help())]
    #[arg(default_value = "dummy:fuzz")]
    pub jobs: Vec<FuzzerJob>,
}

static LOG_LEVEL: LazyLock<RwLock<LogLevel>> = LazyLock::new(|| RwLock::new(LogLevel::Warn));

pub fn get_log_level() -> LogLevel {
    let l = LOG_LEVEL.read().unwrap();
    *l
}

pub fn set_log_level(s: &LogLevel) {
    let mut _l = LOG_LEVEL.write().unwrap();
    *_l = *s;
}

#[macro_export]
macro_rules! debug {
    ($($arg:tt)*) => {
        if $crate::conf::get_log_level() >= $crate::conf::LogLevel::Debug {
            use colored::Colorize;
            println!("{} {}", "[DEBUG]".blue(), format!($($arg)*));
        }
    }
}

#[macro_export]
macro_rules! info {
    ($($arg:tt)*) => {
        if $crate::conf::get_log_level() >= $crate::conf::LogLevel::Info {
            // println!($($arg)*);
            use colored::Colorize;
            println!("{} {}", "[INFO]".green(), format!($($arg)*));
        }
    }
}

#[macro_export]
macro_rules! warn {
    ($($arg:tt)*) => {
        if $crate::conf::get_log_level() >= $crate::conf::LogLevel::Warn {
            // eprintln!($($arg)*);
            use colored::Colorize;
            eprintln!("{} {}", "[WARN]".yellow(), format!($($arg)*));
        }
    }
}

#[macro_export]
macro_rules! error {
    ($($arg:tt)*) => {
        if $crate::conf::get_log_level() >= $crate::conf::LogLevel::Error {
            // eprintln!($($arg)*);
            use colored::Colorize;
            println!("{} {}", "[ERROR]".red(), format!($($arg)*));
        }
    }
}
