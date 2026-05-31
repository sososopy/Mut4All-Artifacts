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

pub struct Add_Lifetime_Parameter_105;

impl Mutator for Add_Lifetime_Parameter_105 {
    fn name(&self) -> &str {
        "Add_Lifetime_Parameter_105"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let lifetime_param = LifetimeParam {
                    lifetimes: BoundLifetimes::None,
                    lifetime: Lifetime {
                        apostrophe: token::Lifetime::default(),
                        ident: Ident::new(&format!("'{}", thread_rng().gen::<char>()), Span::call_site()),
                    },
                };
                trait_item.generics.params.push(GenericParam::Lifetime(lifetime_param));
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(ref_type) = *pat_type.ty {
                                    ref_type.lifetime = Some(Lifetime {
                                        apostrophe: token::Lifetime::default(),
                                        ident: trait_item.generics.params.last().unwrap().ident.clone(),
                                    });
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Struct(struct_item) = item {
                let lifetime_param = LifetimeParam {
                    lifetimes: BoundLifetimes::None,
                    lifetime: Lifetime {
                        apostrophe: token::Lifetime::default(),
                        ident: Ident::new(&format!("'{}", thread_rng().gen::<char>()), Span::call_site()),
                    },
                };
                struct_item.generics.params.push(GenericParam::Lifetime(lifetime_param));
                for field in &mut struct_item.fields {
                    if let syn::Field::Named(field) = field {
                        if let Type::Reference(ref_type) = *field.ty {
                            ref_type.lifetime = Some(Lifetime {
                                apostrophe: token::Lifetime::default(),
                                ident: struct_item.generics.params.last().unwrap().ident.clone(),
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a new lifetime parameter to trait and struct definitions. It then updates the existing code to use this new lifetime parameter, potentially revealing bugs in the Rust compiler related to const generics and lifetimes."
    }
}