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

pub struct Add_Const_Generic_Trait_Constrained_Clone_Impl_92;

impl Mutator for Add_Const_Generic_Trait_Constrained_Clone_Impl_92 {
    fn name(&self) -> &str {
        "Add_Const_Generic_Trait_Constrained_Clone_Impl_92"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let has_type_param = struct_item.generics.params.iter().any(|param| matches!(param, GenericParam::Type(_)));
                if !has_type_param {
                    continue;
                }

                let first_type_param = struct_item.generics.params.iter()
                    .find(|param| matches!(param, GenericParam::Type(_)))
                    .and_then(|param| {
                        if let GenericParam::Type(ty_param) = param {
                            Some(ty_param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .unwrap_or_else(|| Ident::new("T", Span::call_site()));

                if let syn::Data::Struct(data) = &mut struct_item.data {
                    match &mut data.fields {
                        syn::Fields::Named(named) => {
                            named.named.push(parse_quote! {
                                _phantom: std::marker::PhantomData<#first_type_param>
                            });
                        },
                        syn::Fields::Unnamed(unnamed) => {
                            unnamed.unnamed.push(parse_quote! {
                                std::marker::PhantomData<#first_type_param>
                            });
                        },
                        syn::Fields::Unit => {
                            *data = syn::Data::Struct(syn::DataStruct {
                                struct_token: data.struct_token.clone(),
                                fields: syn::Fields::Named(syn::FieldsNamed {
                                    brace_token: Default::default(),
                                    named: {
                                        let mut fields = Punctuated::new();
                                        fields.push(parse_quote! {
                                            _phantom: std::marker::PhantomData<#first_type_param>
                                        });
                                        fields
                                    },
                                }),
                                semi_token: None,
                            });
                        },
                    }
                }

                let trait_path = parse_quote!(HasN);
                let bounded_ty = parse_quote!(#first_type_param);
                let where_pred = syn::WherePredicate::Trait(syn::PredicateTrait {
                    lifetimes: None,
                    bounded_ty: Box::new(bounded_ty),
                    colon_token: Default::default(),
                    path: trait_path,
                    paren_token: None,
                });

                if struct_item.generics.where_clause.is_none() {
                    struct_item.generics.where_clause = Some(syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    });
                }

                struct_item.generics.where_clause.as_mut().unwrap().predicates.push(where_pred);

                let struct_name = &struct_item.ident;
                let impl_item = parse_quote! {
                    impl<U> Clone for #struct_name<U> where [(); U::N]: Sized {
                        fn clone(&self) -> Self {
                            Self { _phantom: self._phantom }
                        }
                    }
                };

                let index = file.items.iter().position(|i| i == item).unwrap();
                file.items.insert(index + 1, syn::Item::Impl(impl_item));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}