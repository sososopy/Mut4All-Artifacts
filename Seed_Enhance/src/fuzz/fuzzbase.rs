use std::{
    any::Any,
    env::temp_dir,
    error::Error,
    ffi::OsStr,
    fmt::Display,
    fs,
    io::Read,
    path::Path,
    process::{Command, Output, Stdio},
    time::Duration,
};

use dyn_clone::DynClone;
use regex::Regex;
use wait_timeout::ChildExt;
use walkdir::WalkDir;

use crate::{conf::Args, debug, fuzz::feature_list::FEATURES, util::glob_range, warn};

const TIMEOUT: u64 = 60; // seconds

#[derive(Debug)]
pub enum FResult {
    CompileSuccess(Output),
    CompileError(Output),
    InternalCompileError(Output),
    HangOnCompile,
    RunSuccess,
    RunError,
}
impl Display for FResult {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::CompileSuccess(_) => write!(f, "Compile Success :)"),
            Self::CompileError(_) => write!(f, "Compile Error :|"),
            Self::InternalCompileError(_) => write!(f, "ICE?!?!?! :("),
            _ => write!(f, "Not defined"),
        }
    }
}
pub trait Fuzzer: Send + Sync + DynClone + Any {
    fn new(conf: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>>
    where
        Self: Sized;
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>>;

    /// Report the ICE to the generator to do guide the
    fn inform_ice(&mut self, _code: &[u8], _is_dup: bool) -> Result<(), Box<dyn Error>> {
        Ok(())
    }
    fn dump(code: &[u8], output: &Path) -> Result<(), Box<dyn Error>>
    where
        Self: Sized,
    {
        fuzzer_dump(code, output)
    }
    fn compile(
        code: &[u8],
        output_source: &Path,
        output_bin: &Path,
        extra_args: &[&str],
    ) -> Result<(Vec<String>, FResult), Box<dyn Error>>
    where
        Self: Sized,
    {
        Self::compile_with_features(code, output_source, output_bin, extra_args, &FEATURES)
    }
    fn compile_with_features(
        code: &[u8],
        output_source: &Path,
        output_bin: &Path,
        extra_args: &[&str],
        features: &[&str],
    ) -> Result<(Vec<String>, FResult), Box<dyn Error>>
    where
        Self: Sized,
    {
        fuzzer_compile::<Self>(code, output_source, output_bin, extra_args, features)
    }

    fn as_mask_fuzzer(&self) -> Result<&dyn MaskFuzzer, Box<dyn Error>> {
        Err("Not a MaskFuzzer".into())
    }
    fn as_infill_fuzzer(&self) -> Result<&dyn InfillFuzzer, Box<dyn Error>> {
        Err("Not a InfillFuzzer".into())
    }
    fn as_mask_fuzzer_mut(&mut self) -> Result<&mut dyn MaskFuzzer, Box<dyn Error>> {
        Err("Not a MaskFuzzer".into())
    }
    fn as_infill_fuzzer_mut(&mut self) -> Result<&mut dyn InfillFuzzer, Box<dyn Error>> {
        Err("Not a InfillFuzzer".into())
    }
}

pub trait InfillFuzzer: Fuzzer {
    fn infill(&mut self, code_prefix: &[u8], code_suffix: &[u8])
    -> Result<Vec<u8>, Box<dyn Error>>;
}

pub trait MaskFuzzer: Fuzzer {
    fn mask(&mut self, code: &[u8]) -> Result<(Vec<u8>, Vec<u8>), Box<dyn Error>>;
    fn dump(code: (Vec<u8>, Vec<u8>), output: &Path) -> Result<(), Box<dyn Error>>
    where
        Self: Sized,
    {
        let (code, mask) = code;
        fs::write(output, code)?;
        fs::write(output.with_extension("mask"), mask)?;
        Ok(())
    }
}

#[derive(Default, Clone)]
pub struct DummyFuzzer {}
impl Fuzzer for DummyFuzzer {
    fn new(_: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        let res = Self {};
        let res = Box::new(res);
        Ok(res)
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        Ok(vec![])
    }
}
impl MaskFuzzer for DummyFuzzer {
    fn mask(&mut self, code: &[u8]) -> Result<(Vec<u8>, Vec<u8>), Box<dyn Error>> {
        let code = code.to_vec();
        let mask = vec![];
        Ok((code, mask))
    }
}
impl InfillFuzzer for DummyFuzzer {
    fn infill(
        &mut self,
        code_prefix: &[u8],
        code_suffix: &[u8],
    ) -> Result<Vec<u8>, Box<dyn Error>> {
        let code = [code_prefix, code_suffix].concat();
        Ok(code)
    }
}

#[derive(Clone)]
pub struct LoadFuzzer {
    pub codes: Vec<Vec<u8>>,
    pub idx: usize,
}
impl Fuzzer for LoadFuzzer {
    fn new(conf: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        let dirs = &conf.input;
        let mut codes = vec![];
        for dir in dirs {
            for f in WalkDir::new(dir) {
                let entry = f?;
                if !entry.file_type().is_file() {
                    continue;
                }
                if entry.path().extension() != Some(OsStr::new("rs")) {
                    continue;
                }
                let code = fs::read(entry.path())?;
                codes.push(code);
            }
        }
        let res = Self { codes, idx: 0 };
        let res = Box::new(res);
        Ok(res)
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        if self.idx >= self.codes.len() {
            warn!("Code index out of range, resetting to 0");
            self.idx = 0;
        }
        let code = self.codes[self.idx].clone();
        // let code = self.codes[glob_range(0..self.codes.len())].clone();
        self.idx += 1;
        Ok(code)
    }
}

pub fn code_mask_feature(code: &[u8]) -> Result<Vec<u8>, Box<dyn Error>> {
    const RE: &str = r"^(#!\[feature\([a-zA-Z0-9_,]*\)\])$";
    const RE_REP: &str = r"\\$1";
    let re = Regex::new(RE)?;
    let res = re
        .replace_all(str::from_utf8(code)?, RE_REP)
        .as_bytes()
        .to_owned();
    Ok(res)
}

pub fn fuzzer_compile<T: Fuzzer>(
    code: &[u8],
    output_source: &Path,
    output_bin: &Path,
    extra_args: &[&str],
    features: &[&str],
) -> Result<(Vec<String>, FResult), Box<dyn Error>> {
    // (Args, Result)
    {
        let code = code_mask_feature(code)?;
        let tmp_file = temp_dir().join(output_source);
        T::dump(&code, &tmp_file)?;
        let args = [
            tmp_file.to_str().unwrap().to_string(),
            "-o".to_string(),
            output_bin.to_str().unwrap().to_string(),
        ];
        let args: Vec<String> = args
            .into_iter()
            .chain(extra_args.iter().map(|s| s.to_string()))
            .collect();
        let extra_args: Vec<String> = features
            .iter()
            .map(|s| format!("-Zcrate-attr=feature({s})"))
            .collect();
        let args: Vec<String> = args.into_iter().chain(extra_args).collect();
        let mut cmd = Command::new("rustc");
        // cmd.arg("+stage2");
        cmd.env("RUST_BACKTRACE", "1");
        cmd.env("RUSTC_ICE", "0");
        cmd.args(args.clone());
        cmd.stdout(Stdio::piped());
        cmd.stderr(Stdio::piped());

        // let mut child = cmd.spawn()?;
        // let timeout = Duration::from_secs(TIMEOUT);
        // let status = child.wait_timeout(timeout)?;
        // match status {
        //     Some(status) => {
        //         let (mut stdout, mut stderr) = (Vec::new(), Vec::new());
        //         match child.stdout {
        //             Some(mut s) => {
        //                 s.read_to_end(&mut stdout)?;
        //             }
        //             None => {}
        //         }
        //         match child.stderr {
        //             Some(mut s) => {
        //                 s.read_to_end(&mut stderr)?;
        //             }
        //             None => {}
        //         }
        //         let code = status.code().ok_or("No code")?;
        //         debug!("Code: {}", code);
        //         let output = Output {
        //             status,
        //             stdout,
        //             stderr,
        //         };
        //         if output.status.success() {
        //             return Ok((args, FResult::CompileSuccess(output)));
        //         }
        //         if code == 1 {
        //             return Ok((args, FResult::CompileError(output)));
        //         }
        //         if code == 101 {
        //             return Ok((args, FResult::InternalCompileError(output)));
        //         }
        //         return Ok((args, FResult::RunError));
        //     }
        //     None => {
        //         child.kill()?;
        //         return Ok((args, FResult::HangOnCompile));
        //     }
        // }

        let status = cmd.output()?;
        let code = status.status.code().ok_or("No code")?;
        debug!("Code: {}", code);
        if status.status.success() {
            return Ok((args, FResult::CompileSuccess(status)));
        }
        if code == 1 {
            return Ok((args, FResult::CompileError(status)));
        }
        Ok((args, FResult::InternalCompileError(status)))
    }
}
pub fn fuzzer_dump(code: &[u8], output: &Path) -> Result<(), Box<dyn Error>> {
    fs::write(output, code)?;
    Ok(())
}
