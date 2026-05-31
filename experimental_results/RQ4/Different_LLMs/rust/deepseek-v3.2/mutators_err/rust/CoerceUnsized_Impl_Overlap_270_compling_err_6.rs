use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, ItemImpl, Local, Pat, PatType, Path as SynPath,
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

pub struct CoerceUnsized_Impl_Overlap_270;

impl Mutator for CoerceUnsized_Impl_Overlap_270 {
    fn name(&self) -> &str {
        "CoerceUnsized_Impl_Overlap_270"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found_coerce_impl = false;
        let mut struct_name = None;
        let mut trait_name = None;
        
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if struct_name.is_none() {
                    struct_name = Some(item_struct.ident.clone());
                }
            }
            if let Item::Trait(item_trait) = item {
                if trait_name.is_none() {
                    trait_name = Some(item_trait.ident.clone());
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    for segment in &type_path.path.segments {
                        if segment.ident == "CoerceUnsized" {
                            found_coerce_2impl = true;
                            let mut new_generics = item_impl.generics.clone();
                            let new_param = GenericParam::Type(TypeParam {
                                attrs: Vec::new(),
                                ident: Ident::new("U", Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                                eq_token: None,
                                default: None,
                            });
                            new_generics.params.push(new_param);
                            item_impl.generics = new_generics;
                            let mut new_where_clause = item_impl.generics.where_clause.clone();
                            if let Some(ref mut clause) = new_where_clause {
                                clause.predicates.clear();
                                if let Some(trait_name_val) = trait_name.clone() {
                                    clause.predicates.push(parse_quote!(T: Unsize<dyn #trait_name_val>));
                                } else {
                                    clause.predicates.push(parse_quote!(T: Unsize<dyn Marker>));
                                }
                            } else {
                                let mut predicates = Punctuated::new();
                                if let Some(trait_name_val) = trait_name.clone() {
                                    predicates.push(parse_quote!(T: Unsize<dyn #trait_name_val>));
                                } else {
                                    predicates.push(parse_quote!(T: Unsize<dyn Marker>));
                                }
                                item_impl.generics.where_clause = Some(WhereClause {
                                    where_token: Default::default(),
                                    predicates,
                                });
                            }
                        }
                    }
                }
            }
        }
        if !found_coerce_impl {
            let struct_name_val = struct_name.unwrap_or_else(|| Ident::new("Box", Span::call_site()));
            let trait_name_val = trait_name.unwrap_or_else(|| Ident::new("Sendable", Span::call_site()));
            let new_struct: ItemStruct = parse_quote! {
                struct #struct_name_val<T>(T);
            };
            file.items.push(Item::Struct(new_struct));
            let new_trait: ItemTrait = parse_quote! {
                trait #trait_name_val {}
            };
            file.items.push(Item::Trait(new_trait));
            let new_impl: ItemImpl = parse_quote! {
                impl<T, U> CoerceUnsized<#struct_name_val<U>> for #struct_name_val<T> where T: Unsize<dyn #trait_name_val> {}
            };
            file.items.push(Item::Impl(new_impl));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets coherence checks involving overlapping trait impls with conditional where clauses, specifically focusing on the CoerceUnsized trait and Unsize bounds. It modifies existing CoerceUnsized impls by adding a new type parameter and adjusting the where clause to use Unsize with a trait object, creating potential overlap scenarios. If no CoerceUnsized impl exists, it introduces a new struct and trait with a mutated impl. This transformation aims to trigger compiler assertion failures during trait selection and coherence checking."
    }
}