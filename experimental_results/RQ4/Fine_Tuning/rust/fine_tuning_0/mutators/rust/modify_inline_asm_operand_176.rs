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

pub struct Modify_Inline_Asm_Operand_176;

impl Mutator for Modify_Inline_Asm_Operand_176 {
    fn name(&self) -> &str {
        "Modify_Inline_Asm_Operand_176"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = InlineAsmVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly blocks within function bodies, specifically modifying the assembly instructions by introducing an operand that is potentially out of bounds or unexpected. This aims to test the compiler's handling of inline assembly, particularly in the context of operand bounds and type mismatches."
    }
}

struct InlineAsmVisitor;

impl VisitMut for InlineAsmVisitor {
    fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*i.func {
            if path.is_ident("asm") {
                if let Some(first_arg) = i.args.first_mut() {
                    if let Expr::Lit(expr_lit) = first_arg {
                        if let syn::Lit::Str(lit_str) = &mut expr_lit.lit {
                            let original_asm = lit_str.value();
                            let modified_asm = original_asm.replace("rsi", "rcx");
                            *lit_str = syn::LitStr::new(&modified_asm, lit_str.span());
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, i);
    }
}