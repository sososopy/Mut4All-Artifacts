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

pub struct Change_Const_Generic_Type_In_Trait_Impl_98;

impl Mutator for Change_Const_Generic_Type_In_Trait_Impl_98 {
    fn name(&self) -> &str {
        "Change_Const_Generic_Type_In_Trait_Impl_98"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_const_types = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &item_trait.items {
                    if let syn::TraitItem::Const(item_const) = trait_item {
                        if let Type::Path(type_path) = &item_const.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                let ident_str = segment.ident.to_string();
                                if ident_str == "usize" || ident_str == "u64" || ident_str == "i32" || ident_str == "u32" || ident_str == "i64" || ident_str == "i16" || ident_str == "u16" || ident_str == "i8" || ident_str == "u8" {
                                    trait_const_types.insert((item_trait.ident.clone(), ident_str));
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for generic_param in &mut item_impl.generics.params {
                    if let GenericParam::Const(const_param) = generic_param {
                        let current_type_str = const_param.ty.to_token_stream().to_string();
                        let mut found_trait = None;
                        if let Some((_, trait_path, _)) = &item_impl.trait_ {
                            for (trait_ident, trait_type_str) in &trait_const_types {
                                if trait_path.segments.last().unwrap().ident == *trait_ident {
                                    found_trait = Some(trait_type_str);
                                    break;
                                }
                            }
                        }
                        if let Some(trait_type_str) = found_trait {
                            if current_type_str != *trait_type_str {
                                let mut rng = thread_rng();
                                let integer_types = ["i32", "u32", "i64", "u64", "i16", "u16", "i8", "u8"];
                                let new_type_str = integer_types.choose(&mut rng).unwrap();
                                if *new_type_str != *trait_type_str && *new_type_str != current_type_str {
                                    const_param.ty = parse_quote!(#new_type_str);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with associated constants where the impl block uses a const generic parameter with a type that differs from the expected type in the trait definition. It changes the const generic parameter type in an impl block to a different integer type that is incompatible with the trait's associated constant type. This transformation aims to trigger compiler bugs related to const generic type mismatches and trait implementation validation."
    }
}