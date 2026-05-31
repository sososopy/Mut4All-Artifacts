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

pub struct Modify_Inline_Assembly_Operand_172;

impl Mutator for Modify_Inline_Assembly_Operand_172 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Operand_172"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Macro(expr_macro)) = stmt {
                        if let Some(ident) = expr_macro.mac.path.get_ident() {
                            if ident == "asm" {
                                let mut tokens = expr_macro.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                                for token in &mut tokens {
                                    if let TokenTree::Literal(lit) = token {
                                        let lit_str = lit.to_string();
                                        if lit_str.contains("global.get 0") {
                                            *lit = Literal::string("global.get 1");
                                        }
                                    }
                                }
                                expr_macro.mac.tokens = tokens.into_iter().collect();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly blocks using `asm!`. It modifies operands within the assembly instructions, such as changing `global.get 0` to `global.get 1`. This transformation tests the compiler's handling of assembly operand variations and explores potential issues related to inline assembly syntax and semantics."
    }
}