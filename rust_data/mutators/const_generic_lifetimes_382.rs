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

pub struct Const_Generic_Lifetimes_382;

impl Mutator for Const_Generic_Lifetimes_382 {
    fn name(&self) -> &str {
        "Const_Generic_Lifetimes_382"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_const_generic = false;
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generic = true;
                        break;
                    }
                }

                if has_const_generic {
                    let lifetime = Lifetime::new("'static", Span::call_site());
                    let const_generic = GenericParam::Const(syn::ConstParam {
                        attrs: vec![],
                        const_token: token::Const(Span::call_site()),
                        ident: Ident::new("Z", Span::call_site()),
                        colon_token: token::Colon(Span::call_site()),
                        ty: Type::Reference(TypeReference {
                            and_token: token::And(Span::call_site()),
                            lifetime: Some(lifetime),
                            mutability: None,
                            elem: Box::new(Type::Path(TypePath {
                                qself: None,
                                path: Ident::new("str", Span::call_site()).into(),
                            })),
                        }),
                        eq_token: None,
                        default: None,
                    });

                    item_fn.sig.generics.params.insert(0, const_generic);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function definitions utilizing const generics. It modifies the function signature by introducing a new const generic parameter that includes a concrete lifetime, specifically 'static. This change is designed to explore interactions between const generics and lifetimes in Rust, potentially revealing issues within the compiler's handling of these features. The operator applies broadly to any function using const generics, thereby maximizing coverage."
    }
}