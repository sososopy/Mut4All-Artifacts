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

pub struct Introduce_Opaque_Recursive_Call_With_Self_Bounded_Lifetime_195;

impl Mutator for Introduce_Opaque_Recursive_Call_With_Self_Bounded_Lifetime_195 {
    fn name(&self) -> &str {
        "Introduce_Opaque_Recursive_Call_With_Self_Bounded_Lifetime_195"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_trait_needed = false;
        let mut dummy_trait_name = None;
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let bounds = &type_impl_trait.bounds;
                        if bounds.is_empty() {
                            continue;
                        }
                        let first_bound = bounds.first().unwrap();
                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                            dummy_trait_name = Some(trait_bound.path.segments.first().unwrap().ident.to_string());
                            dummy_trait_needed = true;
                        }
                    }
                }
            }
        }
        if dummy_trait_needed {
            let dummy_trait_name_str = dummy_trait_name.unwrap_or("Dummy".to_string());
            let dummy_trait_item: Item = parse_quote! {
                trait #dummy_trait_name_str<'a> {}
                impl<'a, T> #dummy_trait_name_str<'a> for T {}
            };
            file.items.insert(0, dummy_trait_item);
        }
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let mut has_lifetime_param = false;
                        for param in &func.sig.generics.params {
                            if let GenericParam::Lifetime(_) = param {
                                has_lifetime_param = true;
                                break;
                            }
                        }
                        if !has_lifetime_param {
                            continue;
                        }
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                let lifetime_name = lifetime_param.lifetime.ident.to_string();
                                let self_bound_exists = lifetime_param.bounds.iter().any(|bound| {
                                    bound.ident.to_string() == lifetime_name
                                });
                                if !self_bound_exists {
                                    lifetime_param.bounds.push(Lifetime::new(&lifetime_name, Span::call_site()));
                                }
                            }
                        }
                        let lifetime_args = func.sig.generics.params.iter()
                            .filter_map(|param| {
                                if let GenericParam::Lifetime(lifetime_param) = param {
                                    Some(lifetime_param.lifetime.clone())
                                } else {
                                    None
                                }
                            })
                            .collect::<Vec<_>>();
                        if lifetime_args.is_empty() {
                            continue;
                        }
                        let func_name = func.sig.ident.clone();
                        let lifetime_arg = lifetime_args.first().unwrap().clone();
                        let let_stmt: Stmt = parse_quote! {
                            let _: &'#lifetime_arg () = #func_name::<'#lifetime_arg>();
                        };
                        func.block.stmts.insert(0, let_stmt);
                        let diverging_expr: Expr = parse_quote! { loop {} };
                        let diverging_stmt = Stmt::Expr(diverging_expr, None);
                        func.block.stmts.push(diverging_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning opaque types with lifetime parameters. It adds self-bounds (e.g., 'a: 'a) to lifetime parameters, inserts a recursive call to the function with explicit lifetime arguments within a let-statement that type-annotates a reference with one of those lifetimes, and ensures the function body ends with a diverging expression. This transformation aims to expose bugs in the compiler's handling of opaque types, recursive calls, and self-bounded lifetimes, particularly in region inference and borrow checking."
    }
}