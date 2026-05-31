use astmasker::ASTMasker;
use quote::ToTokens;
use std::error::Error;
use strategy::item::itemfn::do_gen_main;
use syn::visit_mut::VisitMut;

mod astmasker;
mod strategy;

use syn::{Block, Expr, Item, Pat, Stmt, Type};
use syn::{File, parse_file};

use crate::conf::Args;
use crate::util::glob_range;
use crate::{debug, do_gen_name, register_nodetype};

use crate::fuzz::fuzzbase::{Fuzzer, MaskFuzzer};

use super::bracketsmask::BracketsMask;

#[derive(Clone)]
pub struct SynFuzzer {
    fallback: BracketsMask,
}

register_nodetype!(Expr);
register_nodetype!(Block);
register_nodetype!(Stmt);
register_nodetype!(Item);
register_nodetype!(Pat);
register_nodetype!(Type);

impl Fuzzer for SynFuzzer {
    #[allow(clippy::new_ret_no_self)]
    fn new(_: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        let res = Self {
            fallback: BracketsMask {},
        };
        Ok(Box::new(res))
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        let item_cnt = glob_range(1..10);
        let mut item_v = Vec::new();
        for _ in 0..item_cnt {
            item_v.push(do_gen_name!(Item)?.do_gen()?);
        }
        item_v.push(do_gen_main()?);
        let f = File {
            shebang: None,
            attrs: vec![],
            items: item_v,
        };

        let stream = f.to_token_stream();
        let code = stream.to_string();
        let code = code.into_bytes();
        Ok(code)
    }

    fn as_mask_fuzzer(&self) -> Result<&dyn MaskFuzzer, Box<dyn Error>> {
        Ok(self)
    }
    fn as_mask_fuzzer_mut(&mut self) -> Result<&mut dyn MaskFuzzer, Box<dyn Error>> {
        Ok(self)
    }
}

impl MaskFuzzer for SynFuzzer {
    fn mask(&mut self, code: &[u8]) -> Result<(Vec<u8>, Vec<u8>), Box<dyn Error>> {
        let ori_code = code;
        let code = str::from_utf8(code)?;
        let f = parse_file(code);

        let mut f = match f {
            Ok(f) => f,
            Err(_) => {
                debug!("seems a syntax error occurs, fall back to the brackets mask");
                return self.fallback.mask(code.as_bytes());
            }
        };

        let mut masker = ASTMasker::new();
        masker.visit_file_mut(&mut f);

        match masker.mask_data {
            None => Ok((code.to_string().into_bytes(), Vec::new())),
            Some(mask_data) => {
                let rng = mask_data.byte_range();
                let start = rng.start;
                let end = rng.end;
                let code_prefix = ori_code[0..start].to_vec();
                let code_suffix = ori_code[end..].to_vec();

                Ok((code_prefix, code_suffix))
            }
        }
    }
}
