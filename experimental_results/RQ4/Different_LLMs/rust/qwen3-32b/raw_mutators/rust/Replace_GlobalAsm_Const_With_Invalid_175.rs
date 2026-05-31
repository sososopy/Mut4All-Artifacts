use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_GlobalAsm_Const_With_Invalid_175;

impl Mutator for Replace_GlobalAsm_Const_With_Invalid_175 {
    fn name(&self) -> &str {
        "Replace_GlobalAsm_Const_With_Invalid_175"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(macro_item) = item {
                if macro_item.mac.path.is_ident("global_asm") {
                    let tokens = macro_item.mac.tokens.clone();
                    let token_str = tokens.to_string();
                    let re = Regex::new(r"const\s+[^,)]+").unwrap();
                    if let Some(_) = re.find(&token_str) {
                        let modified = re.replace(&token_str, "const 1 / 0").to_string();
                        let new_tokens = proc_macro2::TokenStream::from_str(&modified).unwrap();
                        macro_item.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `global_asm!` macros with `const` operands. It replaces the first valid constant expression with an invalid one (division by zero). This forces the compiler to evaluate invalid const expressions during macro expansion, potentially exposing bugs in const evaluation or macro argument parsing."
    }
}