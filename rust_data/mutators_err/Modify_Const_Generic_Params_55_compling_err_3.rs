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

pub struct Modify_Const_Generic_Params_55;

impl Mutator for Modify_Const_Generic_Params_55 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Params_55"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstGenericVisitor {
            modified: bool,
        }

        impl VisitMut for ConstGenericVisitor {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if self.modified {
                    return;
                }

                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    if let Some(segment) = path.segments.last() {
                        if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(Expr::Lit(lit)) = arg {
                                    if let syn::Lit::Int(ref mut int_lit) = lit.lit {
                                        let new_value = usize::MAX;
                                        *int_lit = syn::LitInt::new(&new_value.to_string(), int_lit.span());
                                        self.modified = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                syn::visit_mut::visit_expr_call_mut(self, node);
            }
        }

        let mut visitor = ConstGenericVisitor { modified: false };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions using const generics, specifically altering the const generic arguments in function calls. By changing these arguments to boundary values like usize::MAX, it aims to trigger inconsistencies or internal compiler errors related to the const generic system."
    }
}