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

pub struct Omit_Generic_Parameter_In_Turbofish_212;

impl Mutator for Omit_Generic_Parameter_In_Turbofish_212 {
    fn name(&self) -> &str {
        "Omit_Generic_Parameter_In_Turbofish_212"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct Visitor {
            modified: bool,
        }
        impl VisitMut for Visitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Call(ExprCall { func, args, .. }) = expr {
                    if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                        if let Some(last_segment) = path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut last_segment.arguments {
                                if angle_bracketed.args.len() >= 2 {
                                    angle_bracketed.args = Punctuated::from_iter(angle_bracketed.args.iter().take(1).cloned());
                                    self.modified = true;
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = Visitor { modified: false };
        visitor.visit_file_mut(file);
        if !visitor.modified {
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        if let Type::Path(type_path) = &mut **return_type {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                                    if angle_bracketed.args.len() >= 2 {
                                        let mut new_path = type_path.path.clone();
                                        if let Some(last_segment) = new_path.segments.last_mut() {
                                            let mut new_args = angle_bracketed.clone();
                                            new_args.args = Punctuated::from_iter(new_args.args.iter().take(1).cloned());
                                            last_segment.arguments = PathArguments::AngleBracketed(new_args);
                                        }
                                        **return_type = Type::Path(TypePath { qself: None, path: new_path });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls using turbofish syntax with multiple generic arguments. It removes all but the first generic argument from the turbofish, creating a syntactically valid but potentially type-incomplete call. This transformation aims to stress the compiler's generic parameter inference, potentially leading to type inference failures, mismatched generic parameter counts, or internal compiler errors when omitted parameters are const generics or have no default values."
    }
}