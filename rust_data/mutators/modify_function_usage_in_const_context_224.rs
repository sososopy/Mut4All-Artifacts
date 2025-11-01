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

pub struct Modify_Function_Usage_In_Const_Context_224;

impl Mutator for Modify_Function_Usage_In_Const_Context_224 {
    fn name(&self) -> &str {
        "Modify_Function_Usage_In_Const_Context_224"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstContextVisitor<'a> {
            modified: bool,
            rng: &'a mut rand::rngs::ThreadRng,
        }

        impl<'a> VisitMut for ConstContextVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(expr_index) = expr {
                    if let Expr::Path(ExprPath { path, .. }) = &*expr_index.expr {
                        if path.segments.last().unwrap().ident == "fn_value" {
                            if self.rng.gen_bool(0.5) {
                                *expr_index.expr = parse_quote! { some_function() };
                            } else {
                                *expr_index.expr = parse_quote! { local_variable };
                            }
                            self.modified = true;
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut rng = thread_rng();
        let mut visitor = ConstContextVisitor { modified: false, rng: &mut rng };

        for item in &mut file.items {
            if let Item::Fn(ItemFn { block, .. }) = item {
                visitor.visit_block_mut(&mut *block);
            }
        }

        if visitor.modified {
            file.items.insert(0, parse_quote! { let local_variable = 5; });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls within constant expressions or contexts requiring constant values. It replaces such calls with expressions that are not permitted in constant contexts, such as a direct function call or a non-constant variable. This increases the likelihood of triggering bugs related to constant evaluation by exploiting the incorrect use of non-constant values in contexts that require const arguments."
    }
}