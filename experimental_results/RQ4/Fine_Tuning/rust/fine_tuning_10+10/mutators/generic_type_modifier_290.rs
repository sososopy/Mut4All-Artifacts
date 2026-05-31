use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
    TraitItem, TraitItemType,
};

use crate::mutator::Mutator;

pub struct Generic_Type_Modifier_290;

impl Mutator for Generic_Type_Modifier_290 {
    fn name(&self) -> &str {
        "Generic_Type_Modifier_290"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut found_gat = false;
                for item in &mut trait_item.items {
                    if let TraitItem::Type(type_item) = item {
                        if type_item.generics.params.len() == 1 {
                            let new_param: GenericParam = parse_quote! { Extra };
                            type_item.generics.params.push(new_param);
                            found_gat = true;
                        }
                    }
                }
                if found_gat {
                    for item in &mut trait_item.items {
                        if let TraitItem::Fn(method_item) = item {
                            if let ReturnType::Type(_, ty) = &mut method_item.sig.output {
                                if let Type::Path(type_path) = &mut **ty {
                                    if let Some(last_segment) = type_path.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = &mut last_segment.arguments {
                                            angle_bracketed.args.push(parse_quote! { Extra });
                                        }
                                    }
                                }
                            }
                            method_item.sig.generics.params.push(parse_quote! { Extra });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets traits with generic associated types (GATs). It identifies traits defining a generic type and modifies them by introducing a new associated type with an additional parameter. This parameter is used in existing functions within the trait, replacing the original type. This mutation can expose inconsistencies in handling additional generic parameters in GATs, potentially triggering compiler issues related to generic type resolution or instantiation."
    }
}