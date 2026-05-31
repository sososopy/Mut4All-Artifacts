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

pub struct Replace_Const_Parameter_With_Variable_438;

impl Mutator for Replace_Const_Parameter_With_Variable_438 {
    fn name(&self) -> &str {
        "Replace_Const_Parameter_With_Variable_438"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in generics.params.iter_mut() {
                        if let GenericParam::Const(const_param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: const_param.attrs.clone(),
                                ident: const_param.ident.clone(),
                                colon_token: Some(const_param.colon_token.clone()),
                                bounds: Default::default(),
                                default: None,
                                eq_token: Default::default(),
                            });
                            *param = new_param;
                            let new_arg = FnArg::Typed(PatType {
                                attrs: vec![],
                                pat: Box::new(Pat::Ident(syn::PatIdent {
                                    attrs: vec![],
                                    by_ref: None,
                                    mutability: None,
                                    ident: const_param.ident.clone(),
                                    subpat: None,
                                })),
                                colon_token: Default::default(),
                                ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: const_param.ident.clone(),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })),
                            });
                            func.sig.inputs.push(new_arg);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant parameters in generic functions with variable parameters. This transformation tests the compiler's ability to handle changes in parameter types and infer the type of the variable."
    }
}