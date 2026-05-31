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

pub struct Replace_Const_Block_Attr_With_Macro_Invocation_207;

impl Mutator for Replace_Const_Block_Attr_With_Macro_Invocation_207 {
    fn name(&self) -> &str {
        "Replace_Const_Block_Attr_With_Macro_Invocation_207"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstBlockVisitor {
            macros: HashSet::new(),
        };
        visitor.visit_file(&file);
        
        if visitor.macros.is_empty() {
            return;
        }
        
        let mut visitor_mut = ConstBlockMutator {
            macros: visitor.macros,
        };
        visitor_mut.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const blocks containing inner attributes, particularly within function return types involving impl Trait and const trait bounds. It modifies const blocks by replacing inner attributes' values with macro invocations that expand to string literals. This transformation mimics edge cases where macro expansion interacts with incremental compilation in const contexts, potentially triggering resolution issues during attribute processing."
    }
}

struct ConstBlockVisitor {
    macros: HashSet<Ident>,
}

impl<'ast> Visit<'ast> for ConstBlockVisitor {
    fn visit_item_macro(&mut self, i: &'ast ItemMacro) {
        if let Some(ident) = &i.ident {
            self.macros.insert(ident.clone());
        }
        syn::visit::visit_item_macro(self, i);
    }
    
    fn visit_macro(&mut self, mac: &'ast Macro) {
        if let Some(path) = mac.path.get_ident() {
            self.macros.insert(path.clone());
        }
        syn::visit::visit_macro(self, mac);
    }
}

struct ConstBlockMutator {
    macros: HashSet<Ident>,
}

impl VisitMut for ConstBlockMutator {
    fn visit_expr_const_mut(&mut self, expr: &mut ExprConst) {
        if let Expr::Block(block) = &*expr.block {
            for stmt in &block.stmts {
                if let Stmt::Item(Item::Macro(item_macro)) = stmt {
                    if let Some(attr) = item_macro.attrs.first() {
                        if let Meta::NameValue(meta_name_value) = &attr.meta {
                            if let Expr::Lit(expr_lit) = &meta_name_value.value {
                                if let Lit::Str(lit_str) = &expr_lit.lit {
                                    if !self.macros.is_empty() {
                                        let macro_ident = self.macros.iter().next().unwrap().clone();
                                        let new_attr = Attribute {
                                            pound_token: attr.pound_token,
                                            style: attr.style,
                                            bracket_token: attr.bracket_token,
                                            meta: Meta::NameValue(MetaNameValue {
                                                path: meta_name_value.path.clone(),
                                                eq_token: meta_name_value.eq_token,
                                                value: parse_quote! { #macro_ident!() },
                                            }),
                                        };
                                        if let Stmt::Item(Item::Macro(item_macro)) = &mut expr.block.as_mut_block().unwrap().stmts[0] {
                                            item_macro.attrs = vec![new_attr];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_const_mut(self, expr);
    }
}