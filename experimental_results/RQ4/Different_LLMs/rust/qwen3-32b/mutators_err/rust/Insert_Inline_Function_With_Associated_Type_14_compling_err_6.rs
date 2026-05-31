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
use syn::ForeignItem;

use crate::mutator::Mutator;

pub struct Insert_Inline_Function_With_Associated_Type_14;

impl Mutator for Insert_Inline_Function_With_Associated_Type_14 {
    fn name(&self) -> &str {
        "Insert_Inline_Function_With_Associated_Type_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        let module_name = "mutator_module_14";
        let has_extern_type = file.items.iter().any(|item| {
            if let Item::ForeignMod(item_foreign) = item {
                item_foreign.items.iter().any(|item| {
                    if let ForeignItem::Type(ty) = &item {
                        ty.ident == "ExternType"
                    } else {
                        false
                    }
                })
            } else {
                false
            }
        });
        let has_trait = file.items.iter().any(|item| {
            if let Item::Trait(item_trait) = item {
                item_trait.ident == "Trait" && item_trait.items.iter().any(|item| {
                    if let syn::TraitItem::Type(trait_type) = item {
                        trait_type.ident == "Type"
                    } else {
                        false
                    }
                })
            } else {
                false
            }
        });
        if !has_extern_type || !has_trait {
            let module_exists = file.items.iter().any(|item| {
                if let Item::Mod(item_mod) = item {
                    item_mod.ident == module_name
                } else {
                    false
                }
            });
            if !module_exists {
                let new_module = parse_quote! {
                    mod mutator_module_14 {
                        extern {
                            type ExternType;
                        }
                        trait Trait {
                            type Type;
                        }
                    }
                };
                file.items.push(Item::Mod(new_module));
            }
        }
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(first_param) = func.sig.inputs.iter_mut().next() {
                    if let FnArg::Typed(pat_type) = first_param {
                        pat_type.ty = Box::new(parse_quote! { <&mutator_module_14::ExternType as mutator_module_14::Trait>::Type });
                    }
                }
                func.attrs.push(parse_quote! { #[inline] });
                let ty = parse_quote! { &mutator_module_14::ExternType };
                let bound = parse_quote! { mutator_module_14::Trait };
                let mut bounds = Punctuated::new();
                bounds.push(TypeParamBound::Trait(TraitBound {
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: bound,
                    paren_token: Some(Default::default()),
                }));
                let predicate = syn::WherePredicate::Type(syn::PredicateType {
                    bounded_ty: *ty.clone(),
                    colon_token: syn::token::Colon::default(),
                    bounds,
                    ..Default::default()
                });
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(predicate);
                } else {
                    func.sig.generics.where_clause = Some(syn::WhereClause {
                        where_token: token::Where::default(),
                        predicates: Punctuated::from_iter(vec![predicate]),
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}