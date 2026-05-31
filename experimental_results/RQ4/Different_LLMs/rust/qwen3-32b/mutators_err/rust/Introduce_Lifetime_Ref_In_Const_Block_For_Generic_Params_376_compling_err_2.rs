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

pub struct Introduce_Lifetime_Ref_In_Const_Block_For_Generic_Params_376;

impl Mutator for Introduce_Lifetime_Ref_In_Const_Block_For_Generic_Params_376 {
    fn name(&self) -> &str {
        "Introduce_Lifetime_Ref_In_Const_Block_For_Generic_Params_376"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(lifetime) = get_first_lifetime(&func.sig.generics) {
                    let mut visitor = ReplaceConstGenericLitVisitor { lifetime };
                    visitor.visit_block_mut(&mut func.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a dummy reference using the outer function's lifetime within a const generic parameter block. This creates complex interactions between const evaluation and borrowck's region handling, potentially triggering ICEs or type inference errors in generic const expressions."
    }
}

fn get_first_lifetime(generics: &syn::Generics) -> Option<Lifetime> {
    for param in &generics.params {
        if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = param {
            return Some(lifetime.clone());
        }
    }
    None
}

fn is_literal(expr: &Expr) -> bool {
    matches!(expr, Expr::Lit(_))
}

struct ReplaceConstGenericLitVisitor {
    lifetime: Lifetime,
}

impl VisitMut for ReplaceConstGenericLitVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        let lifetime = &self.lifetime;
        if let Expr::Path { path, .. } = &mut *expr_call.func {
            for segment in &mut path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            if is_literal(expr) {
                                let let_stmt: Stmt = parse_quote! {
                                    let _: #lifetime ();
                                };
                                let block_expr: Expr = parse_quote! {{
                                    #let_stmt
                                    #expr
                                }};
                                *arg = GenericArgument::Const(block_expr);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr_call);
    }
}