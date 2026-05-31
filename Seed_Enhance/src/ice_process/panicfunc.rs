use std::{
    collections::HashSet,
    error::Error,
    fs::{self, read_to_string},
};

use crate::{conf::Args, debug, fuzz::fuzzbase::FResult};

use serde_json;

use super::ICEFilter;

static QUERY_PANIC_BEGIN: &str = "thread 'rustc' panicked at ";

fn filter_panic_file(msg: &[u8]) -> Result<Option<Vec<u8>>, Box<dyn Error>> {
    let msg_str = str::from_utf8(msg)?;
    let msg: Vec<_> = msg_str
        .lines()
        .filter_map(|s| -> Option<_> {
            let begin = s.find(QUERY_PANIC_BEGIN)? + QUERY_PANIC_BEGIN.len();
            let end = s.len() - 1;
            let b = &s[begin..end];
            Some(b.as_bytes().to_vec())
        })
        .collect();
    Ok(msg.last().map(|v| v.to_owned()))
}

type FilterData = HashSet<Vec<u8>>;
#[derive(Clone, Debug)]
pub struct PanicFuncFilter {
    existed: FilterData,
}
impl PanicFuncFilter {
    #[allow(clippy::new_ret_no_self)]
    pub fn new() -> Box<dyn ICEFilter> {
        Box::new(Self {
            existed: HashSet::default(),
        })
    }
}
impl ICEFilter for PanicFuncFilter {
    fn filter(&self, info: &FResult) -> bool {
        let info = match info {
            FResult::InternalCompileError(x) => x,
            _ => return false,
        };
        let msg = filter_panic_file(&info.stderr);
        debug!("PanicFuncFilter filtering: {:#?}", msg);
        let msg = match msg {
            Ok(Some(x)) => x,
            _ => return false,
        };
        debug!("The panic function is: \n\t{}", str::from_utf8(&msg).unwrap_or_default());
        self.existed.contains(&msg)
    }
    fn add(&mut self, info: &FResult) -> bool {
        let info = match info {
            FResult::InternalCompileError(x) => x,
            _ => return false,
        };
        let msg = filter_panic_file(&info.stderr);
        let msg = match msg {
            Ok(Some(x)) => x,
            _ => return false,
        };
        self.existed.insert(msg)
    }
    fn reset(&mut self) {
        self.existed.clear();
    }
    fn import(&mut self, args: &Args) -> Result<(), Box<dyn Error>> {
        let p = args.datas.join("func_filters.json");
        if !p.exists() {
            return Ok(());
        }
        let f = read_to_string(p)?;
        let datas: FilterData = serde_json::from_str(&f)?;
        debug!("PanicFunc importing panic filters: {}", datas.len());
        self.existed.extend(datas.into_iter());
        Ok(())
    }
    fn export(&self, args: &Args) -> Result<(), Box<dyn Error>> {
        let datas = serde_json::to_string(&self.existed)?;
        let p = args.datas.join("func_filters.json");
        fs::write(p, datas)?;
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use std::env::temp_dir;

    use crate::{
        fuzz::fuzzbase::{DummyFuzzer, Fuzzer},
        util::gen_alnum,
    };

    use super::*;

    #[test]
    fn test_filter_panic_func() {
        let code = b"fn main() {break rust}";
        let tmp_file = temp_dir().join(gen_alnum(4));
        let tmp_out = temp_dir().join(gen_alnum(4));
        let res = DummyFuzzer::compile(code, &tmp_file, &tmp_out, &[]).unwrap();
        let res = match res.1 {
            FResult::InternalCompileError(x) => x,
            _ => unreachable!(),
        };
        let res = filter_panic_file(&res.stderr).unwrap();

        let expected = b"compiler/rustc_hir_typeck/src/lib.rs:528:10".to_vec();

        let res = res.unwrap();
        assert_eq!(res, expected);
    }
}
