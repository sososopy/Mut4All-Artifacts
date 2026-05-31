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

pub struct Nested_Const_Param_Invocation_345;

impl Mutator for Nested_Const_Param_Invocation_345 {
    fn name(&self) -> &str {
        "Nested_Const_Param_Invocation_345"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_const = func.sig.generics.params.iter().any(|param| {
                    if let syn::GenericParam::Const(_) = param {
                        true
                    } else {
                        false
                    }
                });
                if !has_const {
                    continue;
                }
                let mut const_name = None;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_name = Some(const_param.ident.clone());
                        break;
                    }
                }
                if let Some(name) = const_name {
                    let name_str = name.to_string();
                    let tokens = quote! { substs2::<{ #name_str + 1 }>() };
                    let expr = syn::parse2(tokens).unwrap();
                    if let Some(block) = Some(&mut func.block) {
                        let stmt = syn::Stmt::Expr(expr, None);
                        block.stmts.insert(0, stmt);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let has_const = method.sig.generics.params.iter().any(|param| {
                            if let syn::GenericParam::Const(_) = param {
                                true
                            } else {
                                false
                            }
                        });
                        if !has_const {
                            continue;
                        }
                        let mut const_name = None;
                        for param in &method.sig.generics.params {
                            if let syn::GenericParam::Const(const_param) = param {
                                const_name = Some(const_param.ident.clone());
                                break;
                            }
                        }
                        if let Some(name) = const_name {
                            let name_str = name.to_string();
                            let tokens = quote! { substs2::<{ #name_str + 1 }>() };
                            let expr = syn::parse2(tokens).unwrap();
                            if let Some(block) = Some(&mut method.block) {
                                let stmt = syn::Stmt::Expr(expr, None);
                                block.stmts.insert(0, stmt);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces nested const parameter invocations by inserting a call to a dummy function with a generic const argument that references an existing const parameter. This creates layered const expressions that may expose bugs in const evaluation and trait solver handling of dependencies and scopes."
    }
}