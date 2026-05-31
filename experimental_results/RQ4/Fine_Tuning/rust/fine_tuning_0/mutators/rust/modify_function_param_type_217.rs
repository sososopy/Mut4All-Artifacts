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

pub struct Modify_Function_Param_Type_217;

impl Mutator for Modify_Function_Param_Type_217 {
    fn name(&self) -> &str {
        "Modify_Function_Param_Type_217"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_ref.elem {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: Ident::new("Box", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Type(Type::Slice(type_slice.clone())));
                                                            args
                                                        },
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            });
                                            segments
                                        },
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}