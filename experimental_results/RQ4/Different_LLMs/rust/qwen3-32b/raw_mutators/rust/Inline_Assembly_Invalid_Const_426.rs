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

pub struct Inline_Assembly_Invalid_Const_426;

impl Mutator for Inline_Assembly_Invalid_Const_426 {
    fn name(&self) -> &str {
        "Inline_Assembly_Invalid_Const_426"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InlineAssemblyInvalidConstVisitor {};
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly macros (asm!, global_asm!) that accept const expressions. It replaces a valid const expression in the macro arguments with one that contains a compile-time evaluation error, such as division by zero or overflow. This transformation ensures the macro remains syntactically valid but introduces semantic errors in the const evaluation context, potentially exposing compiler bugs in const evaluation and macro argument handling."
    }
}

struct InlineAssemblyInvalidConstVisitor;

impl VisitMut for InlineAssemblyInvalidConstVisitor {
    fn visit_macro(&mut self, macro_invocation: &mut syn::Macro) {
        if macro_invocation.path.is_ident("asm") || macro_invocation.path.is_ident("global_asm") {
            let macro_str = macro_invocation.tokens.to_string();
            let re = Regex::new(r"const\s+([^,]+)").unwrap();
            let mut matches: Vec<_> = re.find_iter(&macro_str).collect();
            if !matches.is_empty() {
                let mut rng = thread_rng();
                let match_idx = rng.gen_range(0..matches.len());
                let match_info = matches[match_idx];
                let original = match_info.as_str();
                let replacement = "const 1 / 0";
                let new_str = macro_str.replacen(original, replacement, 1);
                if let Ok(tokens) = new_str.parse() {
                    macro_invocation.tokens = tokens;
                }
            }
        }
        syn::visit_mut::visit_macro_mut(self, macro_invocation);
    }
}