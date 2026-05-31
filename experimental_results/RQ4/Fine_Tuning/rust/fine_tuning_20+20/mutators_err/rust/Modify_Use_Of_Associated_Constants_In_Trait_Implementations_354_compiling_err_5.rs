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

pub struct Modify_Use_Of_Associated_Constants_In_Trait_Implementations_354;

impl Mutator for Modify_Use_Of_Associated_Constants_In_Trait_Implementations_354 {
    fn name(&self) -> &str {
        "Modify_Use_Of_Associated_Constants_In_Trait_Implementations_354"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_trait_name = None;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Const(_) = item {
                        target_trait_name = Some(item_trait.ident.clone());
                    }
                }
            }
        }
        if target_trait_name.is_none() {
            return;
        }
        let target_trait_name = target_trait_name.unwrap();
        let mut target_impl_type = None;
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == target_trait_name {
                        if let Type::Path(type_path) = &*item_impl.self_ty {
                            target_impl_type = Some(type_path.path.clone());
                        }
                    }
                }
            }
        }
        if target_impl_type.is_none() {
            return;
        }
        let target_impl_type = target_impl_type.unwrap();
        let mut target_struct = None;
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                if let syn::Fields::Unnamed(fields) = &item_struct.fields {
                    if fields.unnamed.len() == 1 {
                        if let Type::Array(type_array) = &fields.unnamed[0].ty {
                            if let Expr::Path(expr_path) = type_array.len.as_ref() {
                                if expr_path
                                    .path
                                    .segments
                                    .last()
                                    .unwrap()
                                    .ident
                                    == "VALUE"
                                {
                                    target_struct = Some(index);
                                }
                            }
                        }
                    }
                }
            }
        }
        if target_struct.is_none() {
            return;
        }
        let target_struct = target_struct.unwrap();
        let new_struct: ItemStruct = parse_quote! {
            struct Container<[(); get_value::<#target_impl_type>()]> {
                field: u32,
            }
        };
        file.items[target_struct] = Item::Struct(new_struct);
        let new_fn: ItemFn = parse_quote! {
            fn get_value<T: #target_trait_name>() -> usize {
                T::VALUE
            }
        };
        file.items.push(Item::Fn(new_fn));
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that use associated constants in const expression-dependent types. It replaces direct constant usage with a function call that retrieves the constant via a trait-bound generic function, introducing potential inference and evaluation challenges."
    }
}