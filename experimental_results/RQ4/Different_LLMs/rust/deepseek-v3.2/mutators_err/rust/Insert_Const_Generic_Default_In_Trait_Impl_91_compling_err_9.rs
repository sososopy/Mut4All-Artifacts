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

pub struct Insert_Const_Generic_Default_In_Trait_Impl_91;

impl Mutator for Insert_Const_Generic_Default_In_Trait_Impl_91 {
    fn name(&self) -> &str {
        "Insert_Const_Generic_Default_In_Trait_Impl_91"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_defs = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            struct_defs.push((item_struct.ident.clone(), const_param.ident.clone(), default.clone()));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    let struct_name = &type_path.path.segments.last().unwrap().ident;
                    for (def_name, const_param_name, default_expr) in &struct_defs {
                        if struct_name == def_name {
                            let mut new_segments = type_path.path.segments.clone();
                            let last_segment = new_segments.last_mut().unwrap();
                            if last_segment.arguments.is_empty() {
                                let mut args = Punctuated::new();
                                args.push(GenericArgument::Const(default_expr.clone()));
                                last_segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args,
                                    gt_token: token::Gt::default(),
                                });
                                item_impl.self_ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: new_segments,
                                    },
                                }));
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations for generic structs with defaulted const generic parameters. It scans the file for struct definitions that have const generic parameters with default values. When it finds an impl block for such a struct without explicit const generic arguments, it inserts the default constant expression as an explicit argument. This transformation tests the compiler's handling of const generic defaults, trait implementation specialization, and the interaction between generic_const_exprs feature and trait resolution."
    }
}