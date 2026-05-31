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

pub struct Const_Generic_Lifetimes_In_Function_Signatures_285;

impl Mutator for Const_Generic_Lifetimes_In_Function_Signatures_285 {
    fn name(&self) -> &str {
        "Const_Generic_Lifetimes_In_Function_Signatures_285"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics;
                if !generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                }) {
                    continue;
                }
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'static", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                let const_param = GenericParam::Const(ConstParam {
                    attrs: Vec::new(),
                    const_token: token::Const {
                        span: Span::call_site(),
                    },
                    ident: Ident::new("Z", Span::call_site()),
                    colon_token: token::Colon {
                        spans: [Span::call_site()],
                    },
                    ty: Type::Reference(TypeReference {
                        and_token: token::And {
                            spans: [Span::call_site()],
                        },
                        lifetime: Some(Lifetime::new("'static", Span::call_site())),
                        mutability: None,
                        elem: Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("str", Span::call_site())),
                        })),
                    }),
                    eq_token: None,
                    default: None,
                });
                generics.params.insert(0, GenericParam::Lifetime(lifetime_param));
                generics.params.insert(1, const_param);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions using const generics by adding a const generic parameter with a 'static lifetime reference to str. It leverages const generic lifetimes to explore potential compiler bugs related to lifetime resolution, const evaluation, and type inference in the presence of advanced generic constructs."
    }
}