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

pub struct Modify_Param_Type_To_Dyn_Fn_4;

impl Mutator for Modify_Param_Type_To_Dyn_Fn_4 {
    fn name(&self) -> &str {
        "Modify_Param_Type_To_Dyn_Fn_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(i) = item {
                if i.sig.ident == "main" {
                    continue;
                }
                for input in &mut i.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let bound = TypeTraitObject {
                            dyn_token: Some(token::Dyn::default()),
                            bounds: punctuated::Punctuated::from_iter(vec![TypeParamBound::Trait(
                                TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: punctuated::Punctuated::from_iter(vec![
                                            PathSegment {
                                                ident: Ident::new(
                                                    "Fn",
                                                    proc_macro2::Span::call_site(),
                                                ),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: syn::punctuated::Punctuated::from_iter(
                                                            vec![GenericArgument::Type(
                                                Type::Path(TypePath {
                                                    qself: None,
                                                    path: syn::Path::from(Ident::new(
                                                        "i32",
                                                        proc_macro2::Span::call_site(),
                                                    )),
                                                }),
                                            )],
                                                        ),
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            },
                                        ]),
                                    },
                                },
                            )]),
                        };
                        pat_type.ty = Box::new(Type::TraitObject(bound));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms the type of each function parameter into a `dyn Fn<i32>` trait object. This introduces dynamic dispatch and type erasure into function inputs, challenging the compiler's handling of trait objects, lifetime elision, and closure compatibility. It aims to uncover bugs in the interaction between trait-based polymorphism and parameter typing."
    }
}
