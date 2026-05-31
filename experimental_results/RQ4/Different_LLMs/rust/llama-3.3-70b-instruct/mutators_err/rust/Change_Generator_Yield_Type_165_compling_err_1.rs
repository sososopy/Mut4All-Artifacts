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

pub struct Change_Generator_Yield_Type_165;

impl Mutator for Change_Generator_Yield_Type_165 {
    fn name(&self) -> &str {
        "Change_Generator_Yield_Type_165"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                        },
                    }) = &**ty
                    {
                        if segments.len() == 1 && segments[0].ident == "Generator" {
                            let new_type = parse_quote! { String };
                            *ty = Box::new(new_type);
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Yield(yield_expr) = &**expr {
                            if let Expr::Path(path) = &*yield_expr.expr {
                                let new_type = parse_quote! { String };
                                yield_expr.expr = Box::new(parse_quote! { #path.to_string() });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generator functions and changes their yield type to String. It also modifies the yielded values to be converted to String using the to_string method. This transformation tests the compiler's handling of generator yield types and type conversions."
    }
}