use proc_macro2::Span;
use quote::*;
use rand::{seq::SliceRandom,Rng, thread_rng};
use regex::Regex;
use std::{default, ops::Range, panic,fs, process::Command,path::Path,collections::HashSet};
use syn::{
    Expr, ExprCall, Ident, ItemFn, ItemStruct, Lifetime, Local, PatType, Path as SynPath, Stmt,
    Type, TypePath, token::Comma,token::{Paren, Plus},visit_mut::VisitMut,File, Item, visit::Visit,punctuated::Punctuated, spanned::Spanned
};

pub trait Mutator {
    // mutator_name
    fn name(&self) -> &str; 
    // mutator
    fn mutate(&self, file: &mut syn::File);
    // 推理过程
    fn chain_of_thought(&self) -> &str;
}


pub struct MutatorInfo {
    pub mutator: Box<dyn Mutator>,
    pub cot: String,
}

impl MutatorInfo {
    pub fn new(mutator: Box<dyn Mutator>) -> Self {
        MutatorInfo {
            cot: mutator.chain_of_thought().to_string(),
            mutator,
        }
    }
}


