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

pub struct Remove_Inherent_Generic_Args_7;

impl Mutator for Remove_Inherent_Generic_Args_7 {
    fn name(&self) -> &str {
        "Remove_Inherent_Generic_Args_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct RemoveTypeArgsVisitor;
        impl<'ast> VisitMut for RemoveTypeArgsVisitor {
            fn visit_type_mut(&mut self, i: &mut Type) {
                if let Type::Path(type_path) = i {
                    self.visit_type_path_mut(type_path);
                } else {
                    syn::visit_mut::visit_type_mut(self, i);
                }
            }

            fn visit_type_path_mut(&mut self, type_path: &mut TypePath) {
                let segments = &mut type_path.path.segments;
                if segments.len() >= 2 {
                    let first_segment = &mut segments[0];
                    if let PathArguments::AngleBracketed(_) = &first_segment.arguments {
                        first_segment.arguments = PathArguments::None;
                    }
                }
                syn::visit_mut::visit_path_mut(self, &mut type_path.path);
            }
        }

        let mut visitor = RemoveTypeArgsVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}