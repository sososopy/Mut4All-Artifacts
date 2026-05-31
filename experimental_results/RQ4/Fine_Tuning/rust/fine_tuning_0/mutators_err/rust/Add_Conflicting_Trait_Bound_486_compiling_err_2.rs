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
};

use crate::mutator::Mutator;

pub struct Add_Conflicting_Trait_Bound_486;

impl Mutator for Add_Conflicting_Trait_Bound_486 {
    fn name(&self) -> &str {
        "Add_Conflicting_Trait_Bound_486"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_trait_defined = false;
        let new_trait_ident = Ident::new("NewTrait", Span::call_site());

        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    let alias_name = &type_alias.ident;

                    if !new_trait_defined {
                        let new_trait: Item = parse_quote! {
                            pub trait #new_trait_ident {}
                        };
                        file.items.push(new_trait);
                        new_trait_defined = true;
                    }

                    let new_impl: Item = parse_quote! {
                        impl #new_trait_ident for #alias_name {}
                    };
                    file.items.push(new_impl);

                    for item in &mut file.items {
                        if let Item::Impl(item_impl) = item {
                            if let Some((_, path, _)) = &item_impl.trait_ {
                                if path.segments.last().unwrap().ident == *alias_name {
                                    let new_bound: TypeParamBound = parse_quote!(#new_trait_ident);
                                    let mut bounds = Punctuated::new();
                                    bounds.push(new_bound);
                                    let new_where_clause = WhereClause {
                                        where_token: Default::default(),
                                        predicates: {
                                            let mut predicates = Punctuated::new();
                                            predicates.push(syn::WherePredicate::Type(PredicateType {
                                                lifetimes: None,
                                                bounded_ty: syn::Type::Path(TypePath {
                                                    qself: None,
                                                    path: path.clone(),
                                                }),
                                                colon_token: Default::default(),
                                                bounds,
                                            }));
                                            predicates
                                        },
                                    };
                                    item_impl.generics.where_clause = Some(new_where_clause);
                                }
                            }
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