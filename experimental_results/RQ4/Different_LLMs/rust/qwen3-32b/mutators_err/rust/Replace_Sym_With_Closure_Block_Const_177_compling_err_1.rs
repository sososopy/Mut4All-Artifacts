use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprClosure, ExprPath, File, FnArg, GenericArgument, 
    GenericParam, Ident, Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, 
    Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, 
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated,
    spanned::Spanned, token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Sym_With_Closure_Block_Const_177;

impl Mutator for Replace_Sym_With_Closure_Block_Const_177 {
    fn name(&self) -> &str {
        "Replace_Sym_With_Closure_Block_Const_177"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if is_asm_macro(&macro_item.mac.path) {
                    let tokens = macro_item.mac.tokens.clone();
                    let mut iter = tokens.into_iter();
                    while let Some(token) = iter.next() {
                        if let proc_macro2::TokenTree::Ident(ident) = token {
                            if ident == "sym" {
                                // Parse the next tokens as an expression
                                let expr_tokens: proc_macro2::TokenStream = iter.by_ref().collect();
                                let expr: syn::Expr = syn::parse2(expr_tokens).unwrap();
                                if let syn::Expr::Path(expr_path) = expr {
                                    // Check if the path has const generics
                                    if let Some(segment) = expr_path.path.segments.last() {
                                        if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            for arg in &mut args.args {
                                                if let syn::GenericArgument::Const(const_arg) = arg {
                                                    // Replace the const expression
                                                    let original_expr = const_arg.expr.clone();
                                                    let new_expr = parse_quote! {{ || { #original_expr }; 0 }};
                                                    *const_arg.expr = new_expr;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn is_asm_macro(path: &syn::Path) -> bool {
    let last = path.segments.last().unwrap();
    last.ident == "asm" || last.ident == "global_asm"
}