use std::{
    collections::HashSet,
    error::Error,
    fs::{self, read_to_string},
};

use regex::Regex;

use serde_json;

use crate::{conf::Args, debug, fuzz::fuzzbase::FResult};

use super::ICEFilter;

static QUERY_STACK_BEGIN: &str = "query stack during panic";
static QUERY_STACK_END: &str = "end of query stack";
static QUERY_STACK_RE: &str = r"#\d+ \[(\w+)\] .*";

fn filter_query_stack(msg: &[u8]) -> Result<Option<Vec<Vec<u8>>>, Box<dyn Error>> {
    let msg_str = str::from_utf8(msg)?;
    let begin = msg_str.find(QUERY_STACK_BEGIN);
    let end = msg_str.find(QUERY_STACK_END);
    let begin = match begin {
        Some(x) => x,
        None => return Ok(None),
    };
    let end = match end {
        Some(x) => x,
        None => return Ok(None),
    };
    if begin >= end {
        return Ok(None);
    }
    let stack_slice = &msg_str[begin..end];
    let re = Regex::new(QUERY_STACK_RE)?;
    let stack = stack_slice
        .lines()
        .fold(Vec::default(), |mut v, s| match re.captures(s) {
            Some(c) => match c.get(1) {
                Some(m) => {
                    let b = m.as_str().as_bytes().to_owned();
                    v.push(b);
                    v
                }
                None => v,
            },
            None => v,
        });
    if stack.is_empty() {
        return Ok(None);
    }
    Ok(Some(stack))
}

type FilterData = HashSet<Vec<Vec<u8>>>;

#[derive(Clone, Debug)]
pub struct QueryStackFilter {
    existed: FilterData,
}
impl QueryStackFilter {
    #[allow(clippy::new_ret_no_self)]
    pub fn new() -> Box<dyn ICEFilter> {
        Box::new(Self {
            existed: HashSet::default(),
        })
    }
}
impl ICEFilter for QueryStackFilter {
    fn filter(&self, info: &FResult) -> bool {
        let info = match info {
            FResult::InternalCompileError(x) => x,
            _ => return false,
        };
        let stack = filter_query_stack(&info.stderr);
        let stack = match stack {
            Ok(Some(x)) => x,
            _ => return false,
        };
        debug!("The query stacks are: \n\t{}", 
            stack
                .iter()
                .map(|s| str::from_utf8(s).unwrap_or_default())
                .collect::<Vec<_>>()
                .join("\n\t\t")
        );
        self.existed.contains(&stack)
    }
    fn add(&mut self, info: &FResult) -> bool {
        let info = match info {
            FResult::InternalCompileError(x) => x,
            _ => return false,
        };
        let stack = filter_query_stack(&info.stderr);
        let stack = match stack {
            Ok(Some(x)) => x,
            _ => return false,
        };
        self.existed.insert(stack)
    }
    fn reset(&mut self) {
        self.existed.clear();
    }
    fn import(&mut self, args: &Args) -> Result<(), Box<dyn Error>> {
        let p = args.datas.join("panic_filters.json");
        if !p.exists() {
            return Ok(());
        }
        let f = read_to_string(p)?;
        let datas: FilterData = serde_json::from_str(&f)?;
        debug!("QueryStack importing panic filters: {}", datas.len());
        self.existed.extend(datas);
        Ok(())
    }
    fn export(&self, args: &Args) -> Result<(), Box<dyn Error>> {
        let datas = serde_json::to_string(&self.existed)?;
        let p = args.datas.join("panic_filters.json");
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
    fn test_filter_query_stack() {
        let code = b"fn main() {break rust}";
        let tmp_file = temp_dir().join(gen_alnum(4));
        let tmp_out = temp_dir().join(gen_alnum(4));
        let res = DummyFuzzer::compile(code, &tmp_file, &tmp_out, &[]).unwrap();
        let res = match res.1 {
            FResult::InternalCompileError(x) => x,
            _ => unreachable!(),
        };
        let res = filter_query_stack(&res.stderr).unwrap();
        let expected: Vec<Vec<u8>> = vec![b"typeck".to_vec(), b"analysis".to_vec()];

        let res = res.unwrap();
        assert_eq!(res, expected);
    }
}
