use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Invalid_Assembly_Instruction_171;

impl Mutator for Insert_Invalid_Assembly_Instruction_171 {
    fn name(&self) -> &str {
        "Insert_Invalid_Assembly_Instruction_171"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(mac) = item {
                if mac.mac.path.is_ident("global_asm") || mac.mac.path.is_ident("asm") {
                    let mut new_tokens = mac.mac.tokens.clone();
                    let invalid_instruction = quote!(asm("INVALID_INSTRUCTION"));
                    new_tokens.extend(invalid_instruction.into_iter());
                    mac.mac.tokens = new_tokens;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly code constructs and inserts invalid assembly instructions to test the compiler's error handling capabilities."
    }
}