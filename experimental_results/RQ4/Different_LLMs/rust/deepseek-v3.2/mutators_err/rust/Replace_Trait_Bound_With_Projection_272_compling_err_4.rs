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

pub struct Replace_Trait_Bound_With_Projection_272;

impl Mutator for Replace_Trait_Bound_With_Projection_272 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Projection_272"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_info = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                let trait_name = trait_item.ident.clone();
                let mut super_traits = Vec::new();
                for bound in &trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        super_traits.push(trait_bound.path.clone());
                    }
                }
                let mut associated_types = Vec::new();
                for item in &trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        associated_types.push(type_item.ident.clone());
                    }
                }
                trait_info.push((trait_name, super_traits, associated_types));
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                mutate_generics(&mut item_fn.sig.generics, &trait_info);
            }
            if let Item::Impl(impl_item) = item {
                mutate_generics(&mut impl_item.generics, &trait_info);
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        mutate_generics(&mut method.sig.generics, &trait_info);
                    }
                }
            }
            if let Item::Struct(struct_item) = item {
                mutate_generics(&mut struct_item.generics, &trait_info);
            }
            if let Item::Enum(enum_item) = item {
                mutate_generics(&mut enum_item.generics, &trait_info);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with associated type equality constraints by projecting to a supertrait's associated type. It targets where clauses and inline bounds, transforming `T: TraitA<Assoc = Concrete>` to `T: TraitA<Assoc = <T as TraitB>::B>` where TraitB is a supertrait of TraitA and B is an associated type in TraitB. This mutation tests the trait solver's handling of nested associated type projections and equality constraints, potentially triggering resolution failures or bugs in the next-generation trait solver."
    }
}

fn mutate_generics(generics: &mut syn::Generics, trait_info: &[(Ident, Vec<SynPath>, Vec<Ident>)]) {
    if let Some(where_clause) = &mut generics.where_clause {
        for predicate in &mut where_clause.predicates {
            if let syn::WherePredicate::Type(pred_type) = predicate {
                if let Type::Path(type_path) = &pred_type.bounded_ty {
                    let type_name = &type_path.path;
                    for bound in &mut pred_type.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            mutate_trait_bound(trait_bound, type_name, trait_info);
                        }
                    }
                }
            }
        }
    }

    for param in &mut generics.params {
        if let GenericParam::Type(type_param) = param {
            for bound in &mut type_param.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    let type_name = SynPath::from(type_param.ident.clone());
                    mutate_trait_bound(trait_bound, &type_name, trait_info);
                }
            }
        }
    }
}

fn mutate_trait_bound(trait_bound: &mut TraitBound, type_name: &SynPath, trait_info: &[(Ident, Vec<SynPath>, Vec<Ident>)]) {
    let path = &mut trait_bound.path;
    if let Some(last_segment) = path.segments.last_mut() {
        let trait_name = &last_segment.ident;
        for (trait_ident, super_traits, associated_types) in trait_info {
            if trait_ident == trait_name {
                for super_trait_path in super_traits {
                    if let Some(super_segment) = super_trait_path.segments.last() {
                        let super_trait_name = &super_segment.ident;
                        for (super_trait_ident, _, super_associated_types) in trait_info {
                            if super_trait_ident == super_trait_name && !super_associated_types.is_empty() {
                                let super_assoc = super_associated_types[0].clone();
                                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::AssocType(assoc_type) = arg {
                                            let projection: Type = parse_quote! {
                                                <#type_name as #super_trait_name>::#super_assoc
                                            };
                                            assoc_type.ty = projection;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}