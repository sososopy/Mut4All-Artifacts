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

pub struct Modify_Const_Function_Signature_318;

impl Mutator for Modify_Const_Function_Signature_318 {
    fn name(&self) -> &str {
        "Modify_Const_Function_Signature_318"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.constness.is_some() {
                            // Remove the const keyword
                            func.sig.constness = None;

                            // Add an effects parameter
                            let effects_param = FnArg::Typed(PatType {
                                attrs: vec![],
                                pat: Box::new(Pat::Ident(Ident::new("effects", Span::call_site()))),
                                colon_token: token::Colon::default(),
                                ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("Effects", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })),
                            });
                            func.sig.inputs.push(effects_param);
                        } else {
                            // Add the const keyword
                            func.sig.constness = Some(token::Const { span: Span::call_site() });

                            // Add an effects parameter
                            let effects_param = FnArg::Typed(PatType {
                                attrs: vec![],
                                pat: Box::new(Pat::Ident(Ident::new("effects", Span::call_site()))),
                                colon_token: token::Colon::default(),
                                ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("Effects", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })),
                            });
                            func.sig.inputs.push(effects_param);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the signature of constant functions within trait implementations by adding or removing the `const` keyword and adding or modifying the `effects` parameter. This transformation aims to test the compiler's handling of constant functions with effects and its ability to resolve trait implementations under altered type expectations."
    }
}