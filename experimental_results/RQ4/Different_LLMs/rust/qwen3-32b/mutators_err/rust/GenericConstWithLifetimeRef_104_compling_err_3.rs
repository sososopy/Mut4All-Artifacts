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

pub struct GenericConstWithLifetimeRef_104;

impl Mutator for GenericConstWithLifetimeRef_104 {
    fn name(&self) -> &str {
        "GenericConstWithLifetimeRef_104"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(caller) = item {
                // Check if caller has any lifetime parameters
                let has_lifetimes = caller.sig.generics.params.iter().any(|param| param.as_lifetime().is_some());
                if !has_lifetimes {
                    continue;
                }
                
                // Get the first lifetime parameter's ident
                let lifetime_name = caller.sig.generics.params.iter()
                    .find(|param| param.as_lifetime().is_some())
                    .and_then(|param| param.as_lifetime().map(|l| l.lifetime.ident.clone()));
                
                if lifetime_name.is_none() {
                    continue;
                }
                let lifetime_name = lifetime_name.unwrap();
                
                // Process statements in the function body
                for stmt in &mut caller.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call_expr) = &mut *expr {
                            // Get the function path
                            if let Expr::Path(path_expr) = call_expr.func.as_mut() {
                                let function_name = path_expr.path.segments.first().unwrap().ident.to_string();
                                
                                // Find the function definition in the file
                                let function_opt = file.items.iter_mut().find(|item| {
                                    if let Item::Fn(func) = item {
                                        func.sig.ident == function_name
                                    } else {
                                        false
                                    }
                                });
                                
                                if let Some(Item::Fn(function)) = function_opt {
                                    // Check if the function has a const parameter of type usize
                                    let has_const_usize = function.sig.generics.params.iter().any(|param| {
                                        if let Some(const_param) = param.as_const() {
                                            if let Type::Path(type_path) = &*const_param.ty {
                                                if let Some(segment) = type_path.path.segments.first() {
                                                    segment.ident == "usize"
                                                } else {
                                                    false
                                                }
                                            } else {
                                                false
                                            }
                                        } else {
                                            false
                                        }
                                    });
                                    
                                    if has_const_usize {
                                        // Process generic arguments in the call
                                        if let PathArguments::AngleBracketed(angle_args) = &mut path_expr.path.segments.first().unwrap().arguments {
                                            for arg in &mut angle_args.args {
                                                if let GenericArgument::Const(const_arg) = arg {
                                                    if let Expr::Lit(lit_expr) = &*const_arg {
                                                        // Create replacement block with lifetime binding
                                                        let replacement = parse_quote! {
                                                            {
                                                                let _: &'lifetime_name ();
                                                                #lit_expr
                                                            }
                                                        };
                                                        *const_arg = replacement;
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}