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

pub struct Replace_Generic_Const_Expr_With_Invalid_Const_Param_345;

impl Mutator for Replace_Generic_Const_Expr_With_Invalid_Const_Param_345 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Expr_With_Invalid_Const_Param_345"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::Generics::WhereClause(_) | syn::Generics::None = func.sig.generics {
                    continue;
                }
                let generics = &mut func.sig.generics;
                if let syn::Generics::WhereClause(_) = generics {
                    continue;
                }
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(const_expr) = &const_param.default {
                            let new_ident = Ident::new("N", Span::call_site());
                            let new_const_expr = Expr::Path(ExprPath {
                                attrs: vec![],
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: new_ident,
                                        arguments: PathArguments::None,
                                    }]),
                                },
                                qself: None,
                            });
                            const_param.default = Some(new_const_expr);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions with const expressions and replaces a valid const parameter with an invalid one, which is not in scope, to trigger potential bugs in the compiler's handling of generic const expressions."
    }
}