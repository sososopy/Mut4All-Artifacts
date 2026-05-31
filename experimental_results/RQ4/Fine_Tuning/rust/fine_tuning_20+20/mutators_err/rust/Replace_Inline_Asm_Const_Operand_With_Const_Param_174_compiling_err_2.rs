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

pub struct Replace_Inline_Asm_Const_Operand_With_Const_Param_174;

impl Mutator for Replace_Inline_Asm_Const_Operand_With_Const_Param_174 {
    fn name(&self) -> &str {
        "Replace_Inline_Asm_Const_Operand_With_Const_Param_174"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsmFinder {
            found: bool,
        }
        impl<'ast> Visit<'ast> for AsmFinder {
            fn visit_expr(&mut self, i: &'ast Expr) {
                if let Expr::Asm(_) = i {
                    self.found = true;
                }
                syn::visit::visit_expr(self, i);
            }
        }
        struct AsmConstReplacer {
            param_name: Ident,
        }
        impl VisitMut for AsmConstReplacer {
            fn visit_expr_mut(&mut self, i: &mut Expr) {
                if let Expr::Asm(i) = i {
                    i.operands = i
                        .operands
                        .clone()
                        .into_iter()
                        .map(|x| match x {
                            syn::ExprAsmOperand::ConstOperand(_) => {
                                syn::ExprAsmOperand::ConstOperand(self.param_name.clone())
                            }
                            _ => x,
                        })
                        .collect();
                }
                syn::visit_mut::visit_expr_mut(self, i);
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() && item_fn.sig.unsafety.is_some() {
                    let mut finder = AsmFinder { found: false };
                    finder.visit_item_fn(item_fn);
                    if finder.found {
                        let param_name = Ident::new("M", Span::call_site());
                        item_fn.sig.generics.params.push(GenericParam::Const(
                            ConstParam {
                                attrs: Vec::new(),
                                const_token: Default::default(),
                                ident: param_name.clone(),
                                colon_token: Default::default(),
                                ty: Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(Ident::new("usize", Span::call_site())),
                                }),
                                eq_token: None,
                                default: None,
                            },
                        ));
                        let mut replacer = AsmConstReplacer { param_name };
                        replacer.visit_item_fn_mut(item_fn);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async unsafe functions that use inline assembly with a const operand. It adds a const generic parameter to the function and replaces the inline assembly's const operand with this new parameter, thereby testing the compiler's handling of const generics in conjunction with inline assembly."
    }
}