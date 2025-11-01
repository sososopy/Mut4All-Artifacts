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

pub struct Modify_Const_Generic_Params_53;

impl Mutator for Modify_Const_Generic_Params_53 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Params_53"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut modified = false;
                let mut new_generics = Punctuated::new();
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.is_ident("&'static str") {
                                let new_const_param: GenericParam = parse_quote! {
                                    const #const_param.ident: usize
                                };
                                new_generics.push(new_const_param);
                                modified = true;
                            } else {
                                new_generics.push(param.clone());
                            }
                        } else {
                            new_generics.push(param.clone());
                        }
                    } else {
                        new_generics.push(param.clone());
                    }
                }
                if modified {
                    item_fn.sig.generics.params = new_generics;
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, expr)) = &local.init {
                                if let Expr::Path(expr_path) = &**expr {
                                    if let Some(ident) = expr_path.path.get_ident() {
                                        if ident == "A" || ident == "B" {
                                            *stmt = parse_quote! {
                                                let #local.pat = #ident as usize;
                                            };
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
        "This mutation operator modifies const generic parameters in function definitions. It changes const generic parameters of type `&'static str` to `usize`. The function signature and all occurrences within the function where these parameters are referenced are updated to accommodate the type change. This explores the behavior of const generics by involving numeric const parameters rather than string slices, potentially revealing issues or bugs related to const parameter handling in the Rust compiler."
    }
}