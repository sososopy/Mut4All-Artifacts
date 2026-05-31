use std::{error::Error, ffi::OsStr, fs, panic::{self, catch_unwind}, path::Path};

use astmutator::ASTMutator;
use quote::ToTokens;
use syn::{parse_file, visit_mut::VisitMut};
use walkdir::WalkDir;

use crate::{conf::Args, debug, fuzz::fuzzbase::Fuzzer, info, util::glob_range, warn};

mod astmutator;

static KNOWN_OVERFLOW: [&str; 1] = ["#93775"];

// Should we pass the code to avoid stack overflow?
fn avoid_stack_overflow_pass(fname: &Path, code: &str) -> bool {
    // If a line contains more than 100 following characters, we should pass
    const END_BACK: &str = ">)}]";

    for line in code.lines() {
        let mut count = 0;
        for c in line.chars() {
            if END_BACK.contains(c) {
                count += 1;
            } else {
                count = 0;
            }
            if count > 100 {
                return true;
            }
        }
    }
    if code.contains("stack-overflow")
        || code.contains("StackOverflow")
        || code.contains("stack_overflow")
    {
        return true;
    }
    let fs = fname.to_str().unwrap_or_default();
    if fs.contains("stack_overflow")
        || fs.contains("StackOverflow")
        || fs.contains("stack-overflow")
    {
        return true;
    }

    for known in KNOWN_OVERFLOW {
        if code.contains(known) {
            return true;
        }
        if fs.contains(known) {
            return true;
        }
    }
    false
}

#[derive(Clone)]
pub struct NodeMutater {
    pub inner: Box<ASTMutator>,
    pub codes: Vec<Vec<u8>>,
}
impl Fuzzer for NodeMutater {
    fn new(conf: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        let dirs = &conf.input;
        let mut codes = vec![];
        let mut inner = Box::new(ASTMutator::new());
        inner.begin_add();
        for dir in dirs {
            for f in WalkDir::new(dir) {
                let entry = f.map_err(|e| {
                    warn!("Error walking dir {}, {}", dir.display(), e);
                    e
                })?;
                if !entry.file_type().is_file() {
                    continue;
                }
                if entry.path().extension() != Some(OsStr::new("rs")) {
                    continue;
                }
                let code = fs::read(entry.path()).map_err(|e| {
                    warn!("Error reading file {}, {}", entry.path().display(), e);
                    e
                })?;
                let code_str = str::from_utf8(&code)?;

                // Avoid stack overflow
                if avoid_stack_overflow_pass(entry.path(), code_str) {
                    debug!(
                        "Avoid stack overflow pass, file: {}",
                        entry.path().display()
                    );
                    continue;
                }

                let code_f = parse_file(code_str);
                let mut code_f = match code_f {
                    Ok(f) => f,
                    Err(e) => {
                        debug!(
                            "Error parsing file {}, seems a syntax error occures {}",
                            entry.path().display(),
                            e
                        );
                        continue;
                    }
                };

                inner.visit_file_mut(&mut code_f);
                codes.push(code);
            }
        }

        info!("Mutator loaded {} files", codes.len());

        let res = Self { inner, codes };
        let res = Box::new(res);
        Ok(res)
    }

    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        // Randomly select a code, and mutate it

        let mut code_f = loop {
            let idx = glob_range(0..self.codes.len());
            let code = &self.codes[idx];
            let code_str = str::from_utf8(code)?;
            let code_f = parse_file(code_str);
            let code_f = match code_f {
                Ok(f) => f,
                Err(_) => {
                    warn!(
                        "Error parsing code, seems a syntax error occures.\nThis should not happen?!"
                    );
                    continue;
                }
            };
            break code_f;
        };
        self.inner.begin_modify();
        self.inner.visit_file_mut(&mut code_f);
        let default_hook = panic::take_hook();
        panic::set_hook(Box::new(|_| {}));
        let code = catch_unwind(|| {
            let code = prettyplease::unparse(&code_f);
            let code = code.into_bytes();
            code
        })
        .unwrap_or_else(|_| {
            let code = code_f.to_token_stream().to_string();
            let code = code.into_bytes();
            code
        });
        panic::set_hook(default_hook);
        Ok(code)
    }

    fn inform_ice(&mut self, code: &[u8], is_dup: bool) -> Result<(), Box<dyn Error>> {
        let code_str = str::from_utf8(code)?;
        let code_f = parse_file(code_str);
        let mut code_f = match code_f {
            Ok(f) => f,
            Err(_) => {
                warn!("Error parsing code, seems a syntax error occures.");
                return Ok(());
            }
        };
        self.inner.begin_adjust(is_dup);
        self.inner.visit_file_mut(&mut code_f);
        Ok(())
    }
}
