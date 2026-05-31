use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Add_Unused_Lifetime_To_Recursive_Rpit_Return_157;

impl Mutator for Add_Unused_Lifetime_To_Recursive_Rpit_Return_157 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_To_Recursive_Rpit_Return_157"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let ident = &func.sig.ident;
                        let mut has_recursive_call = false;
                        for stmt in &func.block.stmts {
                            if let syn::Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(call_expr) = *expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        if path_expr.path.is_ident(ident) {
                                            has_recursive_call = true;
                                            break;
                                        }
                                    }
                                }
                            } else if let syn::Stmt::Expr(expr, Some(_)) = stmt {
                                if let Expr::Call(call_expr) = *expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        if path_expr.path.is_ident(ident) {
                                            has_recursive_call = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if has_recursive_call {
                            let new_lifetime = LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: Lifetime::new("'a", Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            func.sig.generics.params.push(GenericParam::Lifetime(new_lifetime));
                            
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                let new_bound = TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site()));
                                type_impl_trait.bounds.push(new_bound);
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