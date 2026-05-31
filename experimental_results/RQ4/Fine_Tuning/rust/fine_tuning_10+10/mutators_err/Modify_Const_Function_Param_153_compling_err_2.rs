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

pub struct Modify_Const_Function_Param_153;

impl Mutator for Modify_Const_Function_Param_153 {
    fn name(&self) -> &str {
        "Modify_Const_Function_Param_153"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct FunctionVisitor<'a> {
            modified: bool,
            const_ident: &'a str,
        }

        impl<'a> VisitMut for FunctionVisitor<'a> {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if self.modified {
                    return;
                }
                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    if path.segments.len() > 1 {
                        let mut has_const_param = false;
                        let mut new_args = Punctuated::new();

                        for arg in node.args.iter() {
                            if let Expr::Lit(_) = arg {
                                has_const_param = true;
                                let new_const_expr: Expr = parse_quote! {
                                    const _: usize = 2 * 2
                                };
                                new_args.push(parse_quote!(#self.const_ident));
                                self.modified = true;
                            } else {
                                new_args.push(arg.clone());
                            }
                        }

                        if has_const_param {
                            node.args = new_args;
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, node);
            }
        }

        let mut visitor = FunctionVisitor {
            modified: false,
            const_ident: "NEW_PARAM",
        };

        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function calls with constant parameters, especially those involving generic const expressions. It modifies one of the constant parameters to a more complex compile-time evaluable expression. If no such expression exists, it declares a new const expression using simple arithmetic operations."
    }
}