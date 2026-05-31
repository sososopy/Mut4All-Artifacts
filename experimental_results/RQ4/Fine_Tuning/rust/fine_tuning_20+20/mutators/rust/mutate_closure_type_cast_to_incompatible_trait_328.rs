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

pub struct Mutate_Closure_Type_Cast_To_Incompatible_Trait_328;

impl Mutator for Mutate_Closure_Type_Cast_To_Incompatible_Trait_328 {
    fn name(&self) -> &str {
        "Mutate_Closure_Type_Cast_To_Incompatible_Trait_328"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types = HashSet::new();
        for item in &file.items {
            match item {
                Item::Struct(item_struct) => {
                    existing_types.insert(item_struct.ident.to_string());
                }
                Item::Enum(item_enum) => {
                    existing_types.insert(item_enum.ident.to_string());
                }
                Item::Type(item_type) => {
                    existing_types.insert(item_type.ident.to_string());
                }
                Item::Trait(item_trait) => {
                    existing_types.insert(item_trait.ident.to_string());
                }
                Item::Impl(item_impl) => {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(seg) = path.segments.last() {
                            existing_types.insert(seg.ident.to_string());
                        }
                    }
                }
                Item::TraitAlias(item_trait_alias) => {
                    existing_types.insert(item_trait_alias.ident.to_string());
                }
                _ => {}
            }
        }
        let mut target_type = None;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "Fn" {
                    target_type = Some("Fn".to_string());
                    break;
                }
            }
        }
        if target_type.is_none() {
            if existing_types.contains("Fn") {
                target_type = Some("Fn".to_string());
            } else if existing_types.contains("FnOnce") {
                target_type = Some("FnOnce".to_string());
            } else if existing_types.contains("FnMut") {
                target_type = Some("FnMut".to_string());
            }
        }
        let target_type = target_type.unwrap_or_else(|| "Fn".to_string());
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::TraitObject(type_trait_object) = &**return_type {
                        let mut has_fn_bound = false;
                        for bound in &type_trait_object.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn")
                                    || trait_bound.path.is_ident("FnOnce")
                                    || trait_bound.path.is_ident("FnMut")
                                {
                                    has_fn_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_fn_bound {
                            let mut new_block = item_fn.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(parse_quote! { () });
                            item_fn.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Target a closure with a type cast involving dereferencing, such as `&*s as &str`. Replace the target type in the cast with an incompatible type that exists in the seed program but does not satisfy the required trait bound, like `&SomeOtherType`. This mutation leverages existing types to maximize the chance of triggering compiler bugs related to trait resolution and type checking."
    }
}