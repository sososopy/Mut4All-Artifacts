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

pub struct Inline_Assembly_With_Faulty_Constant_Expressions_175;

impl Mutator for Inline_Assembly_With_Faulty_Constant_Expressions_175 {
    fn name(&self) -> &str {
        "Inline_Assembly_With_Faulty_Constant_Expressions_175"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsmExprCollector {
            exprs: Vec<Expr>,
        }
        impl<'ast> Visit<'ast> for AsmExprCollector {
            fn visit_expr(&mut self, node: &'ast Expr) {
                if matches!(
                    node,
                    Expr::Lit(_) | Expr::Unary(_) | Expr::Binary(_) | Expr::Paren(_)
                ) {
                    self.exprs.push(node.clone());
                }
                syn::visit::visit_expr(self, node);
            }
        }
        let mut rng = thread_rng();
        let mut target_asm: Option<ExprMacro> = None;
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                let macro_name = item_macro.mac.path.segments.last().unwrap().ident.to_string();
                if macro_name == "global_asm" || macro_name == "asm" {
                    target_asm = Some(item_macro.mac.clone());
                    break;
                }
            }
        }
        if let Some(target_asm) = target_asm {
            let mut collector = AsmExprCollector { exprs: Vec::new() };
            collector.visit_macro(&target_asm);
            if !collector.exprs.is_empty() {
                let expr = collector.exprs.choose(&mut rng).unwrap();
                let new_expr = match expr {
                    Expr::Lit(_) => {
                        if rng.gen_bool(0.5) {
                            parse_quote!(1 / 0)
                        } else {
                            parse_quote!(1 << -1)
                        }
                    }
                    Expr::Unary(_) => {
                        if rng.gen_bool(0.5) {
                            parse_quote!(1 / 0)
                        } else {
                            parse_quote!(1 << -1)
                        }
                    }
                    Expr::Binary(_) => {
                        if rng.gen_bool(0.5) {
                            parse_quote!(1 / 0)
                        } else {
                            parse_quote!(1 << -1)
                        }
                    }
                    Expr::Paren(_) => {
                        if rng.gen_bool(0.5) {
                            parse_quote!(1 / 0)
                        } else {
                            parse_quote!(1 << -1)
                        }
                    }
                    _ => return,
                };
                let mut replacer = ExprReplacer {
                    target: expr.clone(),
                    replacement: new_expr,
                };
                for item in &mut file.items {
                    if let Item::Macro(item_macro) = item {
                        if item_macro.mac == target_asm {
                            replacer.visit_macro_mut(&mut item_macro.mac);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets inline assembly macros (`global_asm!` and `asm!`) by replacing a constant expression within the macro with a faulty expression that triggers undefined behavior (e.g., division by zero or shifting by a negative count). This transformation exploits the compiler's constant evaluation mechanism within the context of inline assembly, aiming to provoke internal compiler errors or unexpected behavior due to the mishandling of erroneous constant expressions."
    }
}