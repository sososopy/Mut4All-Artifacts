// A fall back for syn, as sometime we intend to get an error syntax

use std::error::Error;

use crate::{
    fuzz::fuzzbase::{Fuzzer, MaskFuzzer},
    util::glob_range,
};

const REMOVE_P: f64 = 0.1;

const L_PART: &str = "[({";
const R_PART: &str = "])}";
fn corresp_l(ch: char) -> char {
    match ch {
        ')' => '(',
        ']' => '[',
        '}' => '{',
        _ => '?',
    }
}

#[derive(Clone)]
pub struct BracketsMask {}
impl Fuzzer for BracketsMask {
    fn new(_: &crate::conf::Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        Ok(Box::new(Self {}))
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        let code = vec![];
        Ok(code)
    }

    fn as_mask_fuzzer(&self) -> Result<&dyn MaskFuzzer, Box<dyn Error>> {
        Ok(self)
    }
    fn as_mask_fuzzer_mut(&mut self) -> Result<&mut dyn MaskFuzzer, Box<dyn Error>> {
        Ok(self)
    }
}
impl MaskFuzzer for BracketsMask {
    fn mask(&mut self, code: &[u8]) -> Result<(Vec<u8>, Vec<u8>), Box<dyn Error>> {
        let code = str::from_utf8(code)?;
        let mut begin = code.len() - 1;
        let mut end = begin - 1;
        let mut overlap_time = 0usize;
        let mut st = Vec::<(usize, char)>::new();

        for (idx, ch) in code.chars().enumerate() {
            if L_PART.contains(ch) {
                st.push((idx, ch));
            } else if R_PART.contains(ch) {
                let lp = corresp_l(ch);
                let top = st.last().copied();
                match top {
                    None => continue,
                    Some((idx2, ch2)) => {
                        if ch2 != lp {
                            continue;
                        }
                        st.pop();
                        let code_len = idx - idx2 + 1;
                        let codelen_modifier = if code_len <= 20 {
                            1.
                        } else {
                            1. / (code_len as f64 * 0.2)
                        };
                        let overlap_time_modifier = 1. / (overlap_time * 2 + 1) as f64;
                        let renew_p = REMOVE_P * codelen_modifier * overlap_time_modifier;
                        if code_len <= 0 {
                            continue;
                        }
                        if glob_range(0. ..1.) > renew_p {
                            continue;
                        }

                        begin = idx2;
                        end = idx + 1;
                        overlap_time += 1;
                    }
                }
            }
        }

        let code = code.to_string();

        while begin > 0 && !code.is_char_boundary(begin) {
            begin -= 1;
        }
        while end < code.len() && !code.is_char_boundary(end) {
            end += 1;
        }
        let code_prefix = code[..begin].to_string();
        let code_suffix = code[end..].to_string();
        let code_prefix = code_prefix.to_string().into_bytes();
        let code_suffix = code_suffix.to_string().into_bytes();

        Ok((code_prefix, code_suffix))
    }
}
