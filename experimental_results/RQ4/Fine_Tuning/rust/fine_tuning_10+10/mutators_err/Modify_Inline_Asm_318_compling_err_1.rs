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

pub struct Modify_Inline_Asm_318;

impl Mutator for Modify_Inline_Asm_318 {
    fn name(&self) -> &str {
        "Modify_Inline_Asm_318"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Semi(Expr::Macro(ref mut mac), _) = stmt {
                        if mac.mac.path.is_ident("asm") {
                            let mut tokens = mac.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                            if let Some(TokenTree::Literal(ref mut lit)) = tokens.get_mut(0) {
                                let original = lit.to_string();
                                let modified = original.replace("rsi", "rcx");
                                *lit = syn::parse_str(&modified).unwrap();
                            }
                            mac.mac.tokens = tokens.into_iter().collect();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets inline assembly blocks within function bodies. It identifies the `asm!` macro and modifies the assembly instructions by altering the operand to use a register that might not be set up correctly, such as replacing `rsi` with `rcx`. This can lead to operand bounds issues or type mismatches, potentially revealing compiler bugs related to inline assembly handling."
    }
}