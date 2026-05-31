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

pub struct Replace_Const_Generic_With_Helper_Generic_352;

impl Mutator for Replace_Const_Generic_With_Helper_Generic_352 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Helper_Generic_352"
    }
    fn mutate(&self, file: &mut syn::File) {
        let helper_name = "helper";
        let mut helper_added = false;
        for item in &mut file.items {
            if let syn::Item::Fn(helper_func) = item {
                if helper_func.sig.ident == helper_name && helper_func.sig.constness.is_some() {
                    helper_added = true;
                    break;
                }
            }
        }
        if !helper_added {
            let helper_fn = parse_quote! {
                const fn helper<T>() -> usize {
                    std::mem::size_of::<T>() / 2
                }
            };
            file.items.push(syn::Item::Fn(helper_fn));
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &mut local.init {
                            if let Expr::Call(call_expr) = &mut *init.expr {
                                if let Expr::Path(path_expr) = &mut *call_expr.func {
                                    if let Some(last_segment) = path_expr.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            for arg in args.args.iter_mut() {
                                                if let GenericArgument::Const(const_arg) = arg {
                                                    if let Expr::Lit(lit) = &*const_arg {
                                                        let has_type_param = func.sig.generics.params.iter().any(|param| {
                                                            if let GenericParam::Type(type_param) = param {
                                                                type_param.ident == "T"
                                                            } else {
                                                                false
                                                            }
                                                        });
                                                        if !has_type_param {
                                                            func.sig.generics.params.push(parse_quote!(T));
                                                            if func.sig.generics.lt_token.is_none() {
                                                                func.sig.generics.lt_token = Some(token::Lt::default());
                                                            }
                                                            if func.sig.generics.gt_token.is_none() {
                                                                func.sig.generics.gt_token = Some(token::Gt::default());
                                                            }
                                                        }
                                                        let helper_call = parse_quote!(helper::<T>());
                                                        *arg = GenericArgument::Const(Expr::Call(helper_call));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call_expr) = &mut *expr {
                            if let Expr::Path(path_expr) = &mut *call_expr.func {
                                if let Some(last_segment) = path_expr.path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        for arg in args.args.iter_mut() {
                                            if let GenericArgument::Const(const_arg) = arg {
                                                if let Expr::Lit(lit) = &*const_arg {
                                                    let has_type_param = func.sig.generics.params.iter().any(|param| {
                                                        if let GenericParam::Type(type_param) = param {
                                                            type_param.ident == "T"
                                                        } else {
                                                            false
                                                        }
                                                    });
                                                    if !has_type_param {
                                                        func.sig.generics.params.push(parse_quote!(T));
                                                        if func.sig.generics.lt_token.is_none() {
                                                            func.sig.generics.lt_token = Some(token::Lt::default());
                                                        }
                                                        if func.sig.generics.gt_token.is_none() {
                                                            func.sig.generics.gt_token = Some(token::Gt::default());
                                                        }
                                                    }
                                                    let helper_call = parse_quote!(helper::<T>());
                                                    *arg = GenericArgument::Const(Expr::Call(helper_call));
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
        "The mutation operator replaces a concrete const generic parameter in a function call with a call to a new const function that depends on a type parameter. This introduces an unevaluated const expression during monomorphization, potentially exposing compiler bugs in const evaluation and generic parameter resolution. The transformation adds a helper function and modifies the function's generic parameters if necessary to ensure the helper can be used."
    }
}