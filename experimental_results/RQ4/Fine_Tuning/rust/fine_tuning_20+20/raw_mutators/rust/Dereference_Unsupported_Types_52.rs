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

pub struct Dereference_Unsupported_Types_52;

impl Mutator for Dereference_Unsupported_Types_52 {
    fn name(&self) -> &str {
        "Dereference_Unsupported_Types_52"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    let type_str = type_path
                        .path
                        .segments
                        .last()
                        .unwrap()
                        .ident
                        .to_string();
                    if type_str == "str" || type_str == "()" {
                        item_const.expr = Box::new(Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(syn::token::Star::default()),
                            expr: item_const.expr.clone(),
                        }));
                    }
                }
            }
            if let Item::Static(item_static) = item {
                if let Type::Path(type_path) = &*item_static.ty {
                    let type_str = type_path
                        .path
                        .segments
                        .last()
                        .unwrap()
                        .ident
                        .to_string();
                    if type_str == "str" || type_str == "()" {
                        item_static.expr = Box::new(Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: syn::UnOp::Deref(syn::token::Star::default()),
                            expr: item_static.expr.clone(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets constants and statics with non-dereferenceable types like `()` or `str` and applies a dereference operation to their expressions. By introducing an invalid dereference, it aims to trigger compiler errors or ICEs related to type checking and dereference handling, leveraging a known issue with dereferencing unsupported types."
    }
}