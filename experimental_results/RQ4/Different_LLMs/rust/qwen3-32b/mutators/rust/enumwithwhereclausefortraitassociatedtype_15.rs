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

pub struct EnumWithWhereClauseForTraitAssociatedType_15;

impl Mutator for EnumWithWhereClauseForTraitAssociatedType_15 {
    fn name(&self) -> &str {
        "EnumWithWhereClauseForTraitAssociatedType_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs = Vec::new();
        let mut traits_with_assoc_type = Vec::new();

        for item in &mut file.items {
            match item {
                Item::Struct(s) => {
                    structs.push(s.ident.clone());
                }
                Item::Trait(trait_item) => {
                    let trait_name = trait_item.ident.clone();
                    for inner_trait_item in &trait_item.items {
                        if let syn::TraitItem::Type(ty) = inner_trait_item {
                            traits_with_assoc_type.push((
                                trait_name.clone(),
                                ty.ident.clone(),
                            ));
                        }
                    }
                }
                _ => {}
            }
        }

        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if enum_item.generics.where_clause.is_none() {
                    if let Some(struct_name) = structs.first() {
                        if let Some((trait_name, assoc_type_name)) = traits_with_assoc_type.first() {
                            let struct_path = parse_quote!(#struct_name);
                            let ref_type = Type::Reference(syn::TypeReference {
                                and_token: Default::default(),
                                lifetime: None,
                                elem: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: struct_path,
                                })),
                                mutability: None,
                            });

                            let trait_path = parse_quote!(#trait_name);
                            let trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: trait_path,
                            };

                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(trait_bound));

                            let predicate = WherePredicate::Type(PredicateType {
                                lifetimes: None,
                                bounded_ty: ref_type,
                                colon_token: Default::default(),
                                bounds,
                            });

                            let mut where_clause = WhereClause {
                                where_token: Default::default(),
                                predicates: Punctuated::new(),
                            };
                            where_clause.predicates.push(predicate);
                            enum_item.generics.where_clause = Some(where_clause);

                            let variant_ident = Ident::new("Variant", Span::call_site());
                            let variant_type = parse_quote!(<#struct_name as #trait_name>::#assoc_type_name);
                            let variant = Variant {
                                attrs: Vec::new(),
                                ident: variant_ident,
                                discriminant: None,
                                fields: Fields::Unnamed(FieldsUnnamed {
                                    paren_token: Default::default(),
                                    unnamed: Punctuated::from_iter(vec![Field {
                                        attrs: Vec::new(),
                                        vis: Visibility::Inherited,
                                        ident: None,
                                        colon_token: None,
                                        mutability: syn::FieldMutability::None,
                                        ty: variant_type,
                                    }]),
                                }),
                            };
                            enum_item.variants.push(variant);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}