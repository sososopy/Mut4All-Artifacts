use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Trait_Impl_For_Type_Alias_Impl_Trait_485;

impl Mutator for Add_Trait_Impl_For_Type_Alias_Impl_Trait_485 {
    fn name(&self) -> &str {
        "Add_Trait_Impl_For_Type_Alias_Impl_Trait_485"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();
        let mut traits = Vec::new();
        let mut concrete_types = Vec::new();

        for item in &file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(_) = &*type_alias.ty {
                    type_aliases.push(type_alias);
                }
            }
            if let Item::Trait(trait_item) = item {
                traits.push(trait_item);
            }
            if let Item::Struct(struct_item) = item {
                concrete_types.push(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(struct_item.ident.clone()),
                }));
            }
            if let Item::Enum(enum_item) = item {
                concrete_types.push(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(enum_item.ident.clone()),
                }));
            }
        }

        let mut new_impls = Vec::new();
        for type_alias in type_aliases {
            for trait_item in &traits {
                if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    let trait_path = &trait_item.ident;
                    let alias_ident = &type_alias.ident;
                    let mut generic_params_exist = false;
                    let mut bound_opt = None;

                    let generics = &type_alias.generics;
                    if !generics.params.is_empty() {
                        generic_params_exist = true;
                        for param in &generics.params {
                            if let GenericParam::Type(type_param) = param {
                                let bounds = &type_param.bounds;
                                if !bounds.is_empty() {
                                    bound_opt = bounds.first().cloned();
                                    break;
                                }
                            }
                        }
                    }

                    if generic_params_exist {
                        let bound = match bound_opt {
                            Some(bound) => bound,
                            None => continue,
                        };
                        let mut found_concrete = None;
                        for concrete_type in &concrete_types {
                            if let Type::Path(type_path) = concrete_type {
                                let type_name = &type_path.path.segments.last().unwrap().ident;
                                let dummy_predicate: syn::WherePredicate = parse_quote! { #type_name : #bound };
                                if let syn::WherePredicate::Type(pred_type) = dummy_predicate {
                                    if let TypeParamBound::Trait(ref trait_bound) = bound {
                                        let trait_path_str = format!("{}", quote! { #trait_bound });
                                        if trait_path_str.contains("Iterator") && type_name == "Range" {
                                            found_concrete = Some(concrete_type.clone());
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(concrete) = found_concrete {
                            let new_impl: Item = parse_quote! {
                                impl #trait_path for #alias_ident<#concrete> {}
                            };
                            new_impls.push(new_impl);
                        }
                    } else {
                        let new_impl: Item = parse_quote! {
                            impl #trait_path for #alias_ident {}
                        };
                        new_impls.push(new_impl);
                    }
                }
            }
        }

        for new_impl in new_impls {
            file.items.push(new_impl);
        }

        let mut has_feature = false;
        for attr in &file.attrs {
            if let syn::Meta::List(meta_list) = &attr.meta {
                if meta_list.path.is_ident("feature") {
                    if let Ok(nested) = meta_list.parse_args::<syn::Meta>() {
                        if let syn::Meta::Path(path) = nested {
                            if path.is_ident("type_alias_impl_trait") {
                                has_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_feature {
            let feature_attr: syn::Attribute = parse_quote! { #![feature(type_alias_impl_trait)] };
            file.attrs.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies type aliases defined with `impl Trait` (TAIT) and existing traits in the seed program. For each matching pair, it adds an impl block implementing the trait for the type alias. If the alias is generic with a bound, it searches for a concrete type in the seed that satisfies the bound (e.g., Range for Iterator) and uses it to instantiate the generic. This transformation tests the compiler's handling of TAIT in trait implementations, generic instantiation, and bound satisfaction, potentially triggering trait resolution errors, ICEs, or feature gate issues."
    }
}