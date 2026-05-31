use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Union_With_Generic_Const_Expressions_340;

impl Mutator for Modify_Union_With_Generic_Const_Expressions_340 {
    fn name(&self) -> &str {
        "Modify_Union_With_Generic_Const_Expressions_340"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut found_generic_const_expr = false;
                for field in item_union.fields.named.iter_mut() {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Block(expr_block) = type_array.len.as_ref() {
                            if expr_block.block.stmts.len() == 1 {
                                if let Stmt::Expr(Expr::Path(expr_path), _) = &expr_block.block.stmts[0] {
                                    if expr_path.path.segments.len() == 1 && expr_path.path.segments[0].ident == "mem" {
                                        found_generic_const_expr = true;
                                    }
                                }
                            }
                        }
                    }
                }

                if found_generic_const_expr {
                    item_union.generics.params.push(parse_quote!(const N: usize));
                    for field in item_union.fields.named.iter_mut() {
                        if let Type::Array(type_array) = &mut field.ty {
                            type_array.len = Box::new(Expr::Binary(ExprBinary {
                                attrs: vec![],
                                left: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: parse_quote!(N),
                                })),
                                op: syn::BinOp::Add(token::Plus {
                                    spans: [Span::call_site()],
                                }),
                                right: Box::new(type_array.len.clone()),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies union definitions that use generic const expressions by adding a new const generic parameter and incorporating it into the field's type definition. This transformation increases the complexity of type evaluation and tests the compiler's const evaluation logic with enhanced const generic interactions."
    }
}