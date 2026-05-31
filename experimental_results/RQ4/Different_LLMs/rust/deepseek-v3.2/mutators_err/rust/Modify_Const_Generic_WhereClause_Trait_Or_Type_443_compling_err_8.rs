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

pub struct Modify_Const_Generic_WhereClause_Trait_Or_Type_443;

impl Mutator for Modify_Const_Generic_WhereClause_Trait_Or_Type_443 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_WhereClause_Trait_Or_Type_443"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let builtin_traits = vec!["Sized", "Sync", "Send", "Copy", "Clone"];
        let primitive_types = vec!["i32", "u8", "u16", "u32", "u64", "i8", "i16", "i64", "f32", "f64", "bool", "char"];
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::TraitObject(trait_object) = &mut pred_type.bounded_ty {
                                if let Some((const_param, trait_bound)) = extract_const_generic_bound(trait_object) {
                                    let mutation_choice = rng.gen_range(0..2);
                                    match mutation_choice {
                                        0 => {
                                            let new_trait = builtin_traits.choose(&mut rng).unwrap();
                                            if *new_trait != trait_bound {
                                                replace_trait_in_trait_object(trait_object, new_trait);
                                            }
                                        }
                                        1 => {
                                            let new_type = primitive_types.choose(&mut rng).unwrap();
                                            replace_type_in_trait_object(trait_object, new_type);
                                        }
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::TraitObject(trait_object) = &mut pred_type.bounded_ty {
                                        if let Some((const_param, trait_bound)) = extract_const_generic_bound(trait_object) {
                                            let mutation_choice = rng.gen_range(0..2);
                                            match mutation_choice {
                                                0 => {
                                                    let new_trait = builtin_traits.choose(&mut rng).unwrap();
                                                    if *new_trait != trait_bound {
                                                        replace_trait_in_trait_object(trait_object, new_trait);
                                                    }
                                                }
                                                1 => {
                                                    let new_type = primitive_types.choose(&mut rng).unwrap();
                                                    replace_type_in_trait_object(trait_object, new_type);
                                                }
                                                _ => {}
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
        "The mutation operator targets where clauses containing for<const ...> binders in functions and methods. It modifies the trait bound applied to const-generic types by changing it to a different built-in trait (Sized, Sync, Send, Copy, Clone) or alters the const parameter's usage within the bound by changing the array element type (e.g., [(); C] to [i32; C]). This transformation tests the compiler's handling of different trait bounds and type structures under const-generic where clauses, potentially exposing edge cases in const parameter capture and trait resolution for non-lifetime binders."
    }
}

fn extract_const_generic_bound(trait_object: &syn::TypeTraitObject) -> Option<(String, String)> {
    if let Some(bound) = trait_object.bounds.first() {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if let Some(path_segment) = trait_bound.path.segments.last() {
                let trait_name = path_segment.ident.to_string();
                if let Some(binder) = &trait_bound.lifetimes {
                    if let Some(generic_param) = binder.lifetimes.first() {
                        if let GenericParam::Const(const_param) = generic_param {
                            let const_name = const_param.ident.to_string();
                            return Some((const_name, trait_name));
                        }
                    }
                }
            }
        }
    }
    None
}

fn replace_trait_in_trait_object(trait_object: &mut syn::TypeTraitObject, new_trait: &str) {
    if let Some(bound) = trait_object.bounds.first_mut() {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if let Some(path_segment) = trait_bound.path.segments.last_mut() {
                path_segment.ident = Ident::new(new_trait, Span::call_site());
            }
        }
    }
}

fn replace_type_in_trait_object(trait_object: &mut syn::TypeTraitObject, new_type: &str) {
    if let Some(bound) = trait_object.bounds.first_mut() {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if let Some(binder) = &mut trait_bound.lifetimes {
                if let Some(generic_param) = binder.lifetimes.first_mut() {
                    if let GenericParam::Const(const_param) = generic_param {
                        if let Type::Array(array_type) = &mut const_param.ty {
                            if let Type::Path(type_path) = &mut *array_type.elem {
                                if let Some(path_segment) = type_path.path.segments.last_mut() {
                                    path_segment.ident = Ident::new(new_type, Span::call_site());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}