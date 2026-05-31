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

pub struct Add_Constraints_To_Unconstrained_Type_Params_36;

impl Mutator for Add_Constraints_To_Unconstrained_Type_Params_36 {
    fn name(&self) -> &str {
        "Add_Constraints_To_Unconstrained_Type_Params_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut unconstrained_params = Vec::new();
                for param in &item_impl.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        let is_constrained = item_impl.generics.where_clause.as_ref().map_or(false, |clause| {
                            clause.predicates.iter().any(|predicate| {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    predicate_type.bounded_ty == syn::Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(type_param.ident.clone()),
                                    })
                                } else {
                                    false
                                }
                            })
                        });
                        if !is_constrained {
                            unconstrained_params.push(type_param.ident.clone());
                        }
                    }
                }
                
                if !unconstrained_params.is_empty() {
                    let new_trait: syn::ItemTrait = parse_quote! {
                        trait SomeTrait {}
                    };
                    file.items.push(syn::Item::Trait(new_trait));

                    for param in unconstrained_params {
                        let constraint: syn::TypeParamBound = parse_quote!(SomeTrait);
                        let mut bounds = Punctuated::new();
                        bounds.push(constraint);

                        for generic_param in &mut item_impl.generics.params {
                            if let syn::GenericParam::Type(type_param) = generic_param {
                                if type_param.ident == param {
                                    type_param.bounds.extend(bounds.clone());
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