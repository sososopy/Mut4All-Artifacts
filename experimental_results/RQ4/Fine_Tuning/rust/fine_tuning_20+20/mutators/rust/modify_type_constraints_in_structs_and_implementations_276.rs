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

pub struct Modify_Type_Constraints_In_Structs_And_Implementations_276;

impl Mutator for Modify_Type_Constraints_In_Structs_And_Implementations_276 {
    fn name(&self) -> &str {
        "Modify_Type_Constraints_In_Structs_And_Implementations_276"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_names: HashSet<String> = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                struct_names.insert(item_struct.ident.to_string());
            }
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut new_generics = item_struct.generics.clone();
                new_generics.params = new_generics
                    .params
                    .into_iter()
                    .map(|param| {
                        if let GenericParam::Type(type_param) = &param {
                            let mut new_type_param = type_param.clone();
                            new_type_param.bounds.clear();
                            GenericParam::Type(new_type_param)
                        } else {
                            param
                        }
                    })
                    .collect();
                item_struct.generics = new_generics;
            }
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.len() == 1 {
                        let trait_name = path.segments.first().unwrap().ident.to_string();
                        if struct_names.contains(&trait_name) {
                            let mut new_generics = item_impl.generics.clone();
                            new_generics.params = new_generics
                                .params
                                .into_iter()
                                .map(|param| {
                                    if let GenericParam::Type(type_param) = &param {
                                        let mut new_type_param = type_param.clone();
                                        new_type_param.bounds.clear();
                                        GenericParam::Type(new_type_param)
                                    } else {
                                        param
                                    }
                                })
                                .collect();
                            item_impl.generics = new_generics;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with generic type constraints and their corresponding trait implementations. It removes all type constraints from the struct's generic parameters and the trait implementation's generic parameters if the trait being implemented is the struct itself. This transformation challenges the compiler's ability to handle constraint-free generics and may expose weaknesses in type inference, trait resolution, or code generation."
    }
}