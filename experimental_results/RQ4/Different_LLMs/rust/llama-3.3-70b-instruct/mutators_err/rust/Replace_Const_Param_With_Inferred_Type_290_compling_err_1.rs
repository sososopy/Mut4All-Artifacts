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

pub struct Replace_Const_Param_With_Inferred_Type_290;

impl Mutator for Replace_Const_Param_With_Inferred_Type_290 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Inferred_Type_290"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let GenericParam::Const(param) = struct_item.generics.params.iter().next() {
                    if let Some(default_value) = &param.default {
                        let new_param = GenericParam::Const(syn::ConstParam {
                            attrs: param.attrs.clone(),
                            const_token: param.const_token,
                            ident: param.ident.clone(),
                            colon_token: param.colon_token,
                            ty: None,
                            default: default_value.clone(),
                            eq_token: param.eq_token,
                            lifetime: param.lifetime.clone(),
                        });
                        struct_item.generics.params[0] = new_param;
                    }
                }
            }
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = &*pat_type.ty
                        {
                            if let Some(segment) = segments.iter().next() {
                                if segment.ident == "const" {
                                    pat_type.ty = Box::new(Type::Infer(TypeInfer {
                                        underscore_token: Default::default(),
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the explicit type of a constant parameter with an inferred type, effectively removing the explicit type annotation. This can help expose bugs in the Rust compiler related to type inference and constant parameters."
    }
}