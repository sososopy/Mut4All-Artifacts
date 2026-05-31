use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Inline_Assembly_Directives_171;

impl Mutator for Modify_Inline_Assembly_Directives_171 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Directives_171"
    }
    fn mutate(&self, file: &mut syn::File) {
        let asm_regex = Regex::new(r"\bMOV\b").unwrap();
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("global_asm") {
                    if let Some(tt) = item_macro.mac.tokens.clone().into_iter().next() {
                        if let TokenTree::Literal(lit) = tt {
                            let asm_code = lit.to_string();
                            let modified_asm = asm_regex.replace_all(&asm_code, "MOOV");
                            item_macro.mac.tokens = TokenStream::from(TokenTree::Literal(Literal::string(&modified_asm)));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies `global_asm!` macro invocations and modifies inline assembly directives by replacing valid mnemonics with invalid ones. This transformation introduces syntax errors in the assembly code, testing the compiler's resilience to malformed inline assembly and potentially uncovering parsing vulnerabilities."
    }
}