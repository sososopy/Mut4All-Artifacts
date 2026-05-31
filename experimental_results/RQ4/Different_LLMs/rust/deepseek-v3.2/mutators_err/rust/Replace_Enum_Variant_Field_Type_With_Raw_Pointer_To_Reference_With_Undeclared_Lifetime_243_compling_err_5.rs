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

pub struct Replace_Enum_Variant_Field_Type_With_Raw_Pointer_To_Reference_With_Undeclared_Lifetime_243;

impl Mutator for Replace_Enum_Variant_Field_Type_With_Raw_Pointer_To_Reference_With_Undeclared_Lifetime_243 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Field_Type_With_Raw_Pointer_To_Reference_With_Undeclared_Lifetime_243"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found = false;
        for item in &mut file.items {
            if found {
                break;
            }
            if let Item::Enum(item_enum) = item {
                let declared_lifetimes: HashSet<String> = item_enum
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| match param {
                        GenericParam::Lifetime(lifetime_param) => Some(lifetime_param.lifetime.ident.to_string()),
                        _ => None,
                    })
                    .collect();
                let mut candidate_lifetime = 'a';
                while declared_lifetimes.contains(&candidate_lifetime.to_string()) {
                    candidate_lifetime = ((candidate_lifetime as u8) + 1) as char;
                }
                let lifetime_str = candidate_lifetime.to_string();
                for variant in &mut item_enum.variants {
                    if !variant.fields.is_empty() {
                        if let syn::Fields::Unnamed(fields_unnamed) = &mut variant.fields {
                            if let Some(first_field) = fields_unnamed.unnamed.first_mut() {
                                let new_type: Type = parse_quote!(*const & #lifetime_str ());
                                first_field.ty = new_type;
                                found = true;
                                break;
                            }
                        } else if let syn::Fields::Named(fields_named) = &mut variant.fields {
                            if let Some(first_field) = fields_named.named.first_mut() {
                                let new_type: Type = parse_quote!(*const & #lifetime_str ());
                                first_field.ty = new_type;
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions with at least one variant containing a field. It replaces the type of the first field in the first such variant with *const &'a (), where 'a is a lifetime not declared on the enum. This introduces a raw pointer to a reference with an undeclared lifetime, replicating conditions that caused an ICE in pattern analysis. The transformation aims to trigger type mismatches and lifetime resolution errors during compilation."
    }
}