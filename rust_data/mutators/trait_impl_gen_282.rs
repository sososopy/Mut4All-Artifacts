use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Trait_Impl_Gen_282;

impl Mutator for Trait_Impl_Gen_282 {
    fn name(&self) -> &str {
        "Trait_Impl_Gen_282"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let mut has_future_return = false;
                    for item in &item_impl.items {
                        if let syn::ImplItem::Fn(method) = item {
                            if let ReturnType::Type(_, ty) = &method.sig.output {
                                if let Type::Path(TypePath { path, .. }) = &**ty {
                                    if path.segments.iter().any(|seg| seg.ident == "Future") {
                                        has_future_return = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if has_future_return {
                        let new_generic = Ident::new("T", Span::call_site());
                        item_impl.generics.params.push(GenericParam::Type(TypeParam {
                            attrs: vec![],
                            ident: new_generic.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                            eq_token: None,
                            default: None,
                        }));

                        for item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(method) = item {
                                method.sig.generics.params.push(GenericParam::Type(TypeParam {
                                    attrs: vec![],
                                    ident: new_generic.clone(),
                                    colon_token: Some(token::Colon::default()),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: parse_quote!(Default),
                                        }));
                                        bounds
                                    },
                                    eq_token: None,
                                    default: None,
                                }));

                                if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                    if let Type::Path(TypePath { path, .. }) = &mut **ty {
                                        path.segments.push(parse_quote!(#new_generic));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations that have functions returning a Future type. It introduces a new generic parameter to the trait implementation function. The new generic parameter is added to the function signature and the return type, complicating type handling and potentially revealing bugs in type inference or trait resolution."
    }
}