use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Modify_Macro_Invocation_Behavior_580;

impl Mutator for Modify_Macro_Invocation_Behavior_580 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_Behavior_580"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Macro(expr_macro) = &*init.expr {
                                if expr_macro.mac.path.is_ident("never") {
                                    if let Some(expr_inside_macro) = expr_macro.mac.tokens.clone().into_iter().next() {
                                        let condition_var: Local = parse_quote! {
                                            let condition = #expr_inside_macro;
                                        };
                                        let new_macro_call: Stmt = parse_quote! {
                                            let _: bool = never!(condition);
                                        };
                                        new_stmts.push(Stmt::Local(condition_var));
                                        new_stmts.push(new_macro_call);
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations that evaluate boolean expressions, specifically those analogous to `never!`. It modifies the macro invocation by evaluating the expression outside the macro and passing the result as a variable into the macro. This alteration tests the compiler's handling of expression evaluation and macro expansion."
    }
}