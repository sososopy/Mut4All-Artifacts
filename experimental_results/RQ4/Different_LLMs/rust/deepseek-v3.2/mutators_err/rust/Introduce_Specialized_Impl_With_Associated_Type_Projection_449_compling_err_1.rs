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

pub struct Introduce_Specialized_Impl_With_Associated_Type_Projection_449;

impl Mutator for Introduce_Specialized_Impl_With_Associated_Type_Projection_449 {
    fn name(&self) -> &str {
        "Introduce_Specialized_Impl_With_Associated_Type_Projection_449"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_defs = Vec::new();
        let mut generic_impls = Vec::new();
        let mut existing_traits_with_assoc = HashSet::new();
        let mut concrete_types = Vec::new();
        let mut uses_specialization = false;

        for item in &file.items {
            match item {
                Item::Trait(trait_item) => {
                    trait_defs.push(trait_item);
                    for trait_item in &trait_item.items {
                        if let syn::TraitItem::Type(_) = trait_item {
                            existing_traits_with_assoc.insert(trait_item.ident().to_string());
                        }
                    }
                }
                Item::Impl(impl_item) => {
                    if impl_item.trait_.is_some() {
                        if !impl_item.generics.params.is_empty() {
                            generic_impls.push(impl_item);
                        }
                        if impl_item.defaultness.is_some() {
                            uses_specialization = true;
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    concrete_types.push(struct_item.ident.clone());
                }
                Item::Enum(enum_item) => {
                    concrete_types.push(enum_item.ident.clone());
                }
                _ => {}
            }
        }

        if trait_defs.is_empty() || generic_impls.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let target_trait = trait_defs.choose(&mut rng).unwrap();
        let target_impl = generic_impls.choose(&mut rng).unwrap();

        let trait_name = &target_trait.ident;
        let impl_trait = target_impl.trait_.as_ref().unwrap();
        if impl_trait.1.segments.last().unwrap().ident != trait_name {
            return;
        }

        let dummy_trait_name = Ident::new("Dummy", Span::call_site());
        let assoc_name = Ident::new("DummyAssoc", Span::call_site());

        let mut found_existing_trait_with_assoc = None;
        for trait_item in &trait_defs {
            for trait_item in &trait_item.items {
                if let syn::TraitItem::Type(type_item) = trait_item {
                    if existing_traits_with_assoc.contains(&type_item.ident.to_string()) {
                        found_existing_trait_with_assoc = Some((trait_item.ident().clone(), type_item.ident.clone()));
                        break;
                    }
                }
            }
            if found_existing_trait_with_assoc.is_some() {
                break;
            }
        }

        let (bound_trait, bound_assoc) = if let Some((trait_ident, assoc_ident)) = found_existing_trait_with_assoc {
            (trait_ident, assoc_ident)
        } else {
            let dummy_trait_item: Item = parse_quote! {
                trait #dummy_trait_name {
                    type #assoc_name;
                }
            };
            file.items.push(dummy_trait_item);

            if !concrete_types.is_empty() {
                let concrete_type = concrete_types.choose(&mut rng).unwrap();
                let dummy_impl: Item = parse_quote! {
                    impl #dummy_trait_name for #concrete_type {
                        type #assoc_name = ();
                    }
                };
                file.items.push(dummy_impl);
            }
            (dummy_trait_name, assoc_name)
        };

        let mut new_generics = target_impl.generics.clone();
        new_generics.params = Punctuated::new();
        new_generics.params.push(GenericParam::Type(TypeParam {
            ident: Ident::new("W", Span::call_site()),
            colon_token: None,
            bounds: Punctuated::new(),
            eq_token: None,
            default: None,
            attrs: Vec::new(),
        }));

        let mut where_clause = WhereClause {
            where_token: Default::default(),
            predicates: Punctuated::new(),
        };
        where_clause.predicates.push(WherePredicate::Type(PredicateType {
            lifetimes: None,
            bounded_ty: Type::Path(TypePath {
                qself: None,
                path: SynPath::from(Ident::new("W", Span::call_site())),
            }),
            colon_token: Default::default(),
            bounds: {
                let mut bounds = Punctuated::new();
                bounds.push(TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: SynPath::from(bound_trait.clone()),
                }));
                bounds
            },
        }));

        new_generics.where_clause = Some(where_clause);

        let mut trait_path = impl_trait.1.clone();
        if let Some(last_segment) = trait_path.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                args.args.clear();
                let assoc_projection: Type = parse_quote! { <W as #bound_trait>::#bound_assoc };
                args.args.push(GenericArgument::Type(assoc_projection));
            }
        }

        let self_ty = target_impl.self_ty.clone();
        let mut new_impl: ItemImpl = parse_quote! {
            impl #new_generics #trait_path for #self_ty {}
        };

        if uses_specialization {
            new_impl.defaultness = Some(Default::default());
        }

        let new_impl_item = Item::Impl(new_impl);
        file.items.push(new_impl_item);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a specialized impl that uses an associated type projection in its trait bound, targeting the interaction between negative coherence checking and specialization. It first analyzes the seed program to find existing trait definitions and generic impls. If a trait with an associated type exists, it uses that; otherwise, it creates a new dummy trait with an associated type and implements it for a concrete type. The mutation then constructs a new impl that specializes an existing generic impl, using the associated type projection in the trait's type parameter. The new impl is marked 'default' if the seed uses specialization. This transformation modifies the specialization graph and can trigger normalization failures during negative coherence checks, mirroring real-world compiler bugs."
    }
}