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

pub struct Dereference_Unsupported_Types_22;

impl Mutator for Dereference_Unsupported_Types_22 {
    fn name(&self) -> &str {
        "Dereference_Unsupported_Types_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(ref mut const_item) = item {
                if let Type::Path(TypePath { path, .. }) = &*const_item.ty {
                    let type_ident = path.segments.last().unwrap().ident.to_string();
                    if type_ident == "str" || type_ident == "()" {
                        if let Expr::Lit(_) = *const_item.expr {
                            let new_expr: Expr = parse_quote!(*#const_item.expr);
                            const_item.expr = Box::new(new_expr);
                        }
                    }
                }
            } else if let Item::Static(ref mut static_item) = item {
                if let Type::Path(TypePath { path, .. }) = &*static_item.ty {
                    let type_ident = path.segments.last().unwrap().ident.to_string();
                    if type_ident == "str" || type_ident == "()" {
                        if let Expr::Lit(_) = *static_item.expr {
                            let new_expr: Expr = parse_quote!(*#static_item.expr);
                            static_item.expr = Box::new(new_expr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies constants and static variables with types that are inherently non-dereferenceable, such as `()` and `str`. It introduces a dereference operation before expressions assigned to these variables, thereby exploring paths that could lead to compilation errors or internal compiler errors due to the unsuitability of dereferencing such types."
    }
}