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

pub struct Introduce_Complex_Generic_Const_Expressions_150;

impl Mutator for Introduce_Complex_Generic_Const_Expressions_150 {
    fn name(&self) -> &str {
        "Introduce_Complex_Generic_Const_Expressions_150"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_generic_const = false;
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_generic_const = true;
                        break;
                    }
                }

                if !has_generic_const {
                    let ident = Ident::new("N", Span::call_site());
                    item_fn.sig.generics.params.push(GenericParam::Const(
                        syn::ConstParam {
                            attrs: vec![],
                            const_token: token::Const(Span::call_site()),
                            ident,
                            colon_token: token::Colon(Span::call_site()),
                            ty: Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!(usize),
                            }),
                            eq_token: None,
                            default: None,
                        },
                    ));
                }

                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(Local {
                        init: Some(init),
                        ..
                    }) = stmt
                    {
                        if let Expr::Array(expr_array) = &mut *init.expr {
                            let new_expr: Expr = parse_quote!({ N + 4 });
                            expr_array.elems.clear();
                            expr_array.elems.push(new_expr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with constant expressions in array initializations, introduces a generic const parameter to the function if not already present, and modifies the constant expression to depend on this generic parameter, increasing the complexity and potential for compiler evaluation issues."
    }
}