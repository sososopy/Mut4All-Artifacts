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

pub struct Modify_Tuple_Access_To_Nonexistent_Field_64;

impl Mutator for Modify_Tuple_Access_To_Nonexistent_Field_64 {
    fn name(&self) -> &str {
        "Modify_Tuple_Access_To_Nonexistent_Field_64"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TupleAccessVisitor<'a> {
            mutated: bool,
            rng: &'a mut rand::rngs::ThreadRng,
        }

        impl<'a> VisitMut for TupleAccessVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Field(expr_field) = expr {
                    if let Expr::Path(ExprPath { path, .. }) = &*expr_field.base {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident.to_string().starts_with("tuple") {
                                if let Some(index) = expr_field.member.as_index() {
                                    let new_index = index + 1; // Modify to a non-existent field
                                    expr_field.member = syn::Member::Unnamed(syn::Index {
                                        index: new_index,
                                        span: Span::call_site(),
                                    });
                                    self.mutated = true;
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut rng = thread_rng();
        let mut visitor = TupleAccessVisitor {
            mutated: false,
            rng: &mut rng,
        };

        for item in &mut file.items {
            visitor.visit_item_mut(item);
            if visitor.mutated {
                break;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies tuple field accesses and modifies them to access a non-existent field index. This transformation aims to test the compiler's handling of tuple access out-of-bounds and its error reporting mechanisms, potentially leading to compilation errors or unexpected runtime behavior."
    }
}