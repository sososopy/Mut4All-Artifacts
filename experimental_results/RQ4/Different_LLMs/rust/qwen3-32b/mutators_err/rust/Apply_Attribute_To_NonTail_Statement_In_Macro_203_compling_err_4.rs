use proc_macro2::*;
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

pub struct Apply_Attribute_To_NonTail_Statement_In_Macro_203;

struct ApplyAttributeVisitor;

impl VisitMut for ApplyAttributeVisitor {
    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        if block.span().is_macro_expansion() {
            if block.stmts.len() >= 2 {
                let first_stmt = &mut block.stmts[0];
                let attr = parse_quote! { #[cfg(FALSE)] };
                match first_stmt {
                    Stmt::Local(ref mut local) => local.attrs.push(attr),
                    Stmt::Item(item) => {
                        if let Item::Fn(func) = item {
                            func.attrs.push(attr);
                        } else if let Item::Struct(struc) = item {
                            struc.attrs.push(attr);
                        }
                    },
                    _ => {}
                }
            }
        }
        syn::visit_mut::visit_block_mut(self, block);
    }
}

impl Mutator for Apply_Attribute_To_NonTail_Statement_In_Macro_203 {
    fn name(&self) -> &str {
        "Apply_Attribute_To_NonTail_Statement_In_Macro_203"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ApplyAttributeVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}