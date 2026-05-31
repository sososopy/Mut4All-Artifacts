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

pub struct Inconsistent_Trait_Object_Method_Invocation_632;

impl Mutator for Inconsistent_Trait_Object_Method_Invocation_632 {
    fn name(&self) -> &str {
        "Inconsistent_Trait_Object_Method_Invocation_632"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let method_name = method.sig.ident.to_string();
                        let self_type = match method.sig.inputs.first() {
                            Some(FnArg::Receiver(receiver)) => {
                                if receiver.reference.is_some() {
                                    if receiver.mutability.is_some() {
                                        "&mut self"
                                    } else {
                                        "&self"
                                    }
                                } else {
                                    "self"
                                }
                            }
                            _ => continue,
                        };

                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                    if let Some(segment) = path.segments.last() {
                                        if segment.ident == method_name {
                                            let new_self_type = match self_type {
                                                "self" => parse_quote!(&self),
                                                "&self" => parse_quote!(self),
                                                "&mut self" => parse_quote!(&self),
                                                _ => continue,
                                            };
                                            expr_call.args.insert(0, new_self_type);
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
        "The mutator targets method calls on trait objects and alters the self type in the method call to create inconsistencies with the trait definition. By changing the self type, it aims to provoke compiler errors related to incorrect self type handling in trait implementations, potentially uncovering bugs in the trait system."
    }
}