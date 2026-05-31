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

pub struct Swap_Type_And_Const_Generics_In_Struct_396;

impl Mutator for Swap_Type_And_Const_Generics_In_Struct_396 {
    fn name(&self) -> &str {
        "Swap_Type_And_Const_Generics_In_Struct_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_structs = vec![];
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                let mut has_const = false;
                let mut has_type = false;
                for param in &item_struct.generics.params {
                    match param {
                        GenericParam::Const(_) => has_const = true,
                        GenericParam::Type(_) => has_type = true,
                        _ => {}
                    }
                }
                if has_const && has_type {
                    target_structs.push(index);
                }
            }
        }
        if target_structs.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let target_index = target_structs.choose(&mut rng).unwrap();
        if let Item::Struct(item_struct) = &mut file.items[*target_index] {
            let mut const_params = vec![];
            let mut type_params = vec![];
            for param in &item_struct.generics.params {
                match param {
                    GenericParam::Const(_) => const_params.push(param.clone()),
                    GenericParam::Type(_) => type_params.push(param.clone()),
                    _ => {}
                }
            }
            if const_params.is_empty() || type_params.is_empty() {
                return;
            }
            let const_index = rng.gen_range(0..const_params.len());
            let type_index = rng.gen_range(0..type_params.len());
            let mut new_params = Punctuated::new();
            for (index, param) in item_struct.generics.params.iter().enumerate() {
                match param {
                    GenericParam::Const(_) => {
                        if index == const_index {
                            new_params.push(type_params[type_index].clone());
                        } else {
                            new_params.push(param.clone());
                        }
                    }
                    GenericParam::Type(_) => {
                        if index == type_index {
                            new_params.push(const_params[const_index].clone());
                        } else {
                            new_params.push(param.clone());
                        }
                    }
                    _ => {
                        new_params.push(param.clone());
                    }
                }
            }
            item_struct.generics.params = new_params;
            if let Some(where_clause) = &mut item_struct.generics.where_clause {
                let mut new_predicates = Punctuated::new();
                for predicate in &where_clause.predicates {
                    match predicate {
                        WherePredicate::Type(predicate_type) => {
                            let mut new_bounded_ty = None;
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.qself.is_none() {
                                    if let Some(ident) = type_path.path.get_ident() {
                                        for (index, param) in item_struct.generics.params.iter().enumerate() {
                                            match param {
                                                GenericParam::Const(const_param) => {
                                                    if index == type_index && const_param.ident == *ident {
                                                        new_bounded_ty = Some(Type::Infer(TypeInfer {
                                                            underscore_token: Default::default(),
                                                        }));
                                                    }
                                                }
                                                GenericParam::Type(type_param) => {
                                                    if index == const_index && type_param.ident == *ident {
                                                        new_bounded_ty = Some(Type::Infer(TypeInfer {
                                                            underscore_token: Default::default(),
                                                        }));
                                                    }
                                                }
                                                _ => {}
                                            }
                                        }
                                    }
                                }
                            }
                            if let Some(new_ty) = new_bounded_ty {
                                new_predicates.push(WherePredicate::Type(PredicateType {
                                    lifetimes: predicate_type.lifetimes.clone(),
                                    bounded_ty: new_ty,
                                    colon_token: predicate_type.colon_token,
                                    bounds: predicate_type.bounds.clone(),
                                }));
                            } else {
                                new_predicates.push(predicate.clone());
                            }
                        }
                        _ => {
                            new_predicates.push(predicate.clone());
                        }
                    }
                }
                where_clause.predicates = new_predicates;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with both type and const generics. It randomly selects one type generic and one const generic, then swaps their positions in the generic parameter list. Additionally, it updates any where clause predicates that reference the swapped generics by replacing them with inferred types (`_`). This transformation challenges the compiler's ability to resolve generic parameter order and where clause constraints, potentially exposing bugs in generic handling, type inference, and const evaluation."
    }
}