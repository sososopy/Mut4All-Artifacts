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

pub struct Mutate_Trait_Alias_Const_Generics_99;

impl Mutator for Mutate_Trait_Alias_Const_Generics_99 {
    fn name(&self) -> &str {
        "Mutate_Trait_Alias_Const_Generics_99"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items = Vec::new();
        for item in &file.items {
            if let Item::Const(const_item) = item {
                const_items.push(const_item.clone());
            }
        }
        let mut rng = thread_rng();
        let mut new_items_to_insert = Vec::new();
        for item in &mut file.items {
            if let Item::TraitAlias(trait_alias) = item {
                let mut modified = false;
                if let Some(last_param) = trait_alias.generics.params.last_mut() {
                    if let GenericParam::Type(type_param) = last_param {
                        if let Some(last_bound) = type_param.bounds.last_mut() {
                            if let TypeParamBound::Trait(trait_bound) = last_bound {
                                if let PathArguments::AngleBracketed(args) = &mut trait_bound.path.segments.last_mut().unwrap().arguments {
                                    let mut const_args = Vec::new();
                                    let mut type_args = Vec::new();
                                    for arg in args.args.iter() {
                                        if let GenericArgument::Const(const_expr) = arg {
                                            const_args.push(const_expr.clone());
                                        } else if let GenericArgument::Type(type_) = arg {
                                            type_args.push(type_.clone());
                                        }
                                    }
                                    if const_args.len() > 0 {
                                        if !type_args.is_empty() && rng.gen_bool(0.5) {
                                            args.args.clear();
                                            for type_arg in type_args {
                                                args.args.push(GenericArgument::Type(type_arg));
                                            }
                                            for const_arg in const_args {
                                                args.args.push(GenericArgument::Const(const_arg));
                                            }
                                            modified = true;
                                        }
                                        if const_items.len() > -1 && rng.gen_bool(0.5) {
                                            let chosen_const = const_items.choose(&mut rng).unwrap();
                                            let const_name = chosen_const.ident.clone();
                                            let new_expr: Expr = parse_quote! { #const_name };
                                            for arg in args.args.iter_mut() {
                                                if let GenericArgument::Const(const_expr) = arg {
                                                    *const_expr = new_expr.clone();
                                                    modified = true;
                                                    break;
                                                }
                                            }
                                        } else {
                                            for arg in args.args.iter_mut() {
                                                if let GenericArgument::Const(const_expr) = arg {
                                                    let new_expr: Expr = parse_quote! { 1 };
                                                    *const_expr = new_expr;
                                                    modified = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if !modified && trait_alias.generics.params.len() > 0 {
                    let mut has_const_param = false;
                    let mut has_type_param = false;
                    for param in &trait_alias.generics.params {
                        if let GenericParam::Const(_) = param {
                            has_const_param = true;
                        } else if let GenericParam::Type(_) = param {
                            has_type_param = true;
                        }
                    }
                    if has_const_param && has_type_param {
                        let mut new_params = Vec::new();
                        for param in trait_alias.generics.params.iter() {
                            if let GenericParam::Type(_) = param {
                                new_params.push(param.clone());
                            }
                        }
                        for param in trait_alias.generics.params.iter() {
                            if let GenericParam::Const(_) = param {
                                new_params.push(param.clone());
                            }
                        }
                        trait_alias.generics.params = Punctuated::from_iter(new_params);
                        modified = true;
                    }
                    if has_const_param && !has_type_param {
                        trait_alias.generics.params.push(parse_quote!(T: ()));
                        modified = true;
                    }
                }
                if !modified {
                    let new_const_item: Item = parse_quote! { const DUMMY: usize = 1; };
                    new_items_to_insert.push(new_const_item);
                    if let Some(last_param) = trait_alias.generics.params.last_mut() {
                        if let GenericParam::Type(type_param) = last_param {
                            if let Some(last_bound) = type_param.bounds.last_mut() {
                                if let TypeParamBound::Trait(trait_bound) = last_bound {
                                    if let PathArguments::AngleBracketed(args) = &mut trait_bound.path.segments.last_mut().unwrap().arguments {
                                        for arg in args.args.iter_mut() {
                                            if let GenericArgument::Const(const_expr) = arg {
                                                let new_expr: Expr = parse_quote! { DUMMY };
                                                *const_expr = new_expr;
                                                break;
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
        for item in new_items_to_insert {
            file.items.insert(0, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait alias definitions involving const generic parameters. It modifies const argument expressions by replacing them with different const expressions (either using existing const items or trivial values), swaps order of const and type parameters in trait alias generics, duplicates const parameters as dummy type parameters, and introduces new const items if needed. This transformation stresses the compiler's const generics system, trait alias resolution, and const evaluation in generic contexts."
    }
}