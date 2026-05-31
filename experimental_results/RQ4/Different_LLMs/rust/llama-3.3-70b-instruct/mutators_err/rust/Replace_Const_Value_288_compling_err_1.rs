use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Const_Value_288;

impl Mutator for Replace_Const_Value_288 {
    fn name(&self) -> &str {
        "Replace_Const_Value_288"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                            ..
                        },
                        ..
                    }) = &**return_type
                    {
                        for segment in segments {
                            if let Some(args) = &segment.arguments {
                                if let PathArguments::AngleBracketed(args) = args {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Const(arg) = arg {
                                            let new_const = if let Expr::Lit(lit) = &**arg {
                                                let new_value = match lit.lit {
                                                    Lit::Int(int) => {
                                                        let mut rng = thread_rng();
                                                        let new_int: i32 =
                                                            rng.gen_range(0..100);
                                                        Lit::Int(syn::LitInt::new(
                                                            &new_int.to_string(),
                                                            int.base,
                                                            int.suffix,
                                                        ))
                                                    }
                                                    Lit::Float(float) => {
                                                        let mut rng = thread_rng();
                                                        let new_float: f64 =
                                                            rng.gen_range(0.0..100.0);
                                                        Lit::Float(syn::LitFloat::new(
                                                            &new_float.to_string(),
                                                            float.suffix,
                                                        ))
                                                    }
                                                    _ => panic!("Unsupported literal type"),
                                                };
                                                Expr::Lit(new_value)
                                            } else {
                                                panic!("Unsupported expression type");
                                            };
                                            *arg = Expr::Const(ExprConst {
                                                expr: new_const,
                                                minus_token: None,
                                                span: arg.span(),
                                            });
                                        }
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
        "The mutation operator replaces constant values used in generic type parameters with new random constant values of the same type. This transformation aims to test the compiler's handling of constant values in generic type parameters, especially in scenarios that may trigger internal compiler errors or warnings related to the feature adt_const_params."
    }
}