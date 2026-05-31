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

pub struct Insert_Lifetime_Parameter_285;

impl Mutator for Insert_Lifetime_Parameter_285 {
    fn name(&self) -> &str {
        "Insert_Lifetime_Parameter_285"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param = LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime {
                        apostrophe: token::Lifetime::default(),
                        ident: Ident::new("a", Span::call_site()),
                    },
                    colon_token: Some(token::Colon::default()),
                    bounds: BoundLifetimes::None,
                };
                func.sig.generics.params.insert(0, GenericParam::Lifetime(lifetime_param));
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ref_type) = &*pat_type.ty {
                            ref_type.lifetime = Some(Lifetime {
                                apostrophe: token::Lifetime::default(),
                                ident: Ident::new("a", Span::call_site()),
                            });
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: Lifetime {
                                apostrophe: token::Lifetime::default(),
                                ident: Ident::new("a", Span::call_site()),
                            },
                            colon_token: Some(token::Colon::default()),
                            bounds: BoundLifetimes::None,
                        };
                        func.sig.generics.params.insert(0, GenericParam::Lifetime(lifetime_param));
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(ref_type) = &*pat_type.ty {
                                    ref_type.lifetime = Some(Lifetime {
                                        apostrophe: token::Lifetime::default(),
                                        ident: Ident::new("a", Span::call_site()),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a lifetime parameter into the generic parameter list of functions and methods. It also updates the lifetime of reference types in the function signature to reference the new lifetime parameter. This transformation aims to test the compiler's handling of lifetimes and const generics, potentially triggering internal compiler errors or type inference issues."
    }
}