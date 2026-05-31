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

pub struct Add_Extra_Param_In_Trait_Default_Method_457;

impl Mutator for Add_Extra_Param_In_Trait_Default_Method_457 {
    fn name(&self) -> &str {
        "Add_Extra_Param_In_Trait_Default_Method_457"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let outer_trait_generics = &trait_item.generics;
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let Some(_) = &method.default {
                            let param_type = if let Some(first_param) = outer_trait_generics.params.first() {
                                if let syn::GenericParam::Type(type_param) = first_param {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(type_param.ident.clone()),
                                    })
                                } else {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("i32", Span::call_site())),
                                    })
                                }
                            } else {
                                Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(Ident::new("i32", Span::call_site())),
                                })
                            };
                            let param_name = Ident::new("_extra", Span::call_site());
                            let new_arg = FnArg::Typed(PatType {
                                attrs: Vec::new(),
                                pat: Box::new(Pat::Ident(PatIdent {
                                    attrs: Vec::new(),
                                    by_ref: None,
                                    mutability: None,
                                    ident: param_name,
                                    subpat: None,
                                })),
                                colon_token: Default::default(),
                                ty: Box::new(param_type),
                            });
                            method.sig.inputs.push(new_arg);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an extraneous parameter to default method implementations in traits, using a type parameter from the trait's context. This creates a signature mismatch between the trait declaration and its implementation, potentially exposing errors in specialization logic or MIR optimization under `-Zpolymorphize`."
    }
}